#include <iostream>
#include <random>

#include "model.hpp"
#include "view.hpp"
#include "controller.hpp"

Coord Game::get_rand_coord(){

    std::uniform_int_distribution<int> dist (0, available.size() - 1);

    auto it = available.begin();
    std::advance(it, dist(Game::generator));

    return *it;
}

void Game::update_available(){
    auto v = View::get();

    Coord max = v -> get_max_coord();
    max.x -= 1;
    max.y -= 1;

    Coord min = v -> get_min_coord();

    for (int i = min.x; i <= max.x; i++)
        for (int j = min.y; j <= max.y; j++)
            available.insert({i, j});

    for (auto& c : swamp)
        available.erase(c);

    for (auto& s : snakes)
        for (auto& seg : s.body)
            available.erase(seg.first);
}

void Game::add_rabbit(){
    rabbits.insert(Rabbit(get_rand_coord(), 3));
}

Game::Game(){
    auto v = View::get();
    v -> draw_all = std::bind(&Game::draw_all, this);
    v -> game_tick = std::bind(&Game::snakes_step, this);

    update_available();
    for (int i = 0; i < num_rabbits; i++){
        add_rabbit();
    }

    Coord start = {v -> get_max_coord().x / 3, v -> get_max_coord().y / 3};

    snakes.push_back(Snake(start));
    start.x += 5;
    snakes.push_back(Snake(start));

    for (int i = 0; i < 7; i++) {
        swamp.insert({20 + i, 40});
    }
}

void Game::draw_all(){
    auto v = View::get();

    v -> draw(swamp);

    for (auto r : rabbits)
        v -> draw(r);

    for (auto s : snakes) {
        v -> draw(s);
    }


}

void Game::snakes_step(){

    std::set<Rabbit> temp;

    for (auto r = rabbits.begin(); r != rabbits.end(); r++) { //slowly drown rabbits in swamp
        auto res = swamp.find(r -> place);
        if (res != swamp.end()) {
            if (r -> left_to_live > 1) 
                temp.insert(Rabbit(r -> place, r -> left_to_live - 1));
        } else {
            temp.insert(Rabbit(r -> place, 5));
        }
    }

    rabbits = temp;

    for (Snake& snake : snakes) { //move snakes

        Coord new_head = snake.body.front().first;

        if (snake.miss_tick) {
            snake.miss_tick--;
        } else {
            switch (snake.body.front().second) {
                case Snake::dir::RIGHT:
                    new_head.y++;
                    break;
                case Snake::dir::UP:
                    new_head.x--;
                    break;
                case Snake::dir::LEFT:
                    new_head.y--;
                    break;
                case Snake::dir::DOWN:
                    new_head.x++;
                    break;
            }
            snake.body.push_front(std::make_pair(new_head, snake.body.front().second));
            if (snake.slowed)
                snake.miss_tick++;

            bool am = false;
            for (auto eaten : rabbits) {
                if (eaten.place == new_head) {
                    rabbits.erase(eaten);
                    add_rabbit();
                    am = true;
                    break;
                }
            }
            if (!am) {
                snake.body.pop_back();
            }
            snake.last_dir = snake.body.front().second;

            for (auto& seg : snake.body) {
                if (swamp.find(seg.first) != swamp.end()) {
                    if (snake.slowed != true) {
                        snake.slowed = true;
                        snake.miss_tick = 1;
                    } 
                } else {
                    snake.slowed = false;
                }
            }
        }
    }

    snakes_check_crash();
    grow_swamp();
}

void Game::snakes_check_crash(){
    auto v = View::get();
    for (std::list<Snake>::iterator next_snake = snakes.begin(); next_snake != snakes.end();) {
        auto snake = next_snake;
        next_snake++;

        Coord head = snake -> body.front().first;
        if ((head.x < v -> get_min_coord().x) || (head.x >= v -> get_max_coord().x - 1)) {
            snakes.erase(snake);
            continue;
        }

        if ((head.y < v -> get_min_coord().y) || (head.y >= v -> get_max_coord().y)) {
            snakes.erase(snake);
            continue;
        }
        
        bool dead = false;
        for (auto other = snakes.begin(); (other != snakes.end()) && !dead; other++) {
            auto seg = other -> body.begin();
            if (other == snake)
                seg = std::next(seg, 1);
            for (;seg != other -> body.end(); seg++)
                if ((head.x == seg -> first.x) && (head.y == seg -> first.y)) {
                    snakes.erase(snake);
                    dead = true;
                    break;
                }
        }

    }
}

void Game::grow_swamp(){
    update_available();

    std::uniform_int_distribution<int> dist_more (0, 3);
    int more = dist_more(Game::generator);

    std::uniform_int_distribution<int> dist_less (0, 2);
    int less = dist_less(Game::generator);

    std::uniform_int_distribution<int> dist_swamp (0, swamp.size() - 1);

    for (int i = 0; i < more; i++) {

        auto cell = swamp.begin();
        std::advance(cell, dist_swamp(Game::generator));

        std::list<Coord> free_space;
        if (available.find(Coord(cell -> x - 1, cell -> y)) != available.end())
            free_space.push_back(Coord(cell -> x - 1, cell -> y));
        if (available.find(Coord(cell -> x + 1, cell -> y)) != available.end())
            free_space.push_back(Coord(cell -> x + 1, cell -> y));
        if (available.find(Coord(cell -> x, cell -> y - 1)) != available.end())
            free_space.push_back(Coord(cell -> x, cell -> y - 1));
        if (available.find(Coord(cell -> x, cell -> y + 1)) != available.end())
            free_space.push_back(Coord(cell -> x, cell -> y - 1));
         
        if (free_space.size() == 0) {
            more++;
            continue;
        }

        std::uniform_int_distribution<int> dist_free (0, free_space.size() - 1);
        auto add = free_space.begin();
        std::advance(add, dist_free(Game::generator));
        swamp.insert(*add);
    }


    for (int i = 0; i < less; i++) {
        std::uniform_int_distribution<int> dist_free (0, swamp.size() - 1);
        auto cell = swamp.begin();
        std::advance(cell, dist_free(Game::generator));
        swamp.erase(cell);
    }
}

Snake::Snake(Coord head) {
    Coord cur = head;
    for (int i = 0; i < 5; i++){
        body.push_back(std::pair<Coord, dir>(cur, dir::RIGHT));
        cur.y--;
    }
    last_dir = dir::RIGHT;
}
