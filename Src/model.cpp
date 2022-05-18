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

    for (auto& c : obstacles)
        available.erase(c);

    for (auto& s : snakes)
        for (auto& seg : s.body)
            available.erase(seg.first);

}

void Game::add_rabbit(){
    rabbits.insert(get_rand_coord());
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
}

void Game::draw_all(){
    auto v = View::get();

    for (auto r : rabbits)
        v -> draw(r);

    for (auto s : snakes) {
        v -> draw(s);
    }

}

void Game::snakes_step(){
    for (Snake& snake : snakes) {

        Coord new_head = snake.body.front().first;

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

        auto eaten = rabbits.find(new_head);
        if (eaten != rabbits.end()) {
            rabbits.erase(eaten);
        } else {
            snake.body.pop_back();
        }
        
        snake.body.push_front(std::make_pair(new_head, snake.body.front().second));

        snake.last_dir = snake.body.front().second;


    }

    snakes_check_crash();
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
        
        for (auto& seg : snake -> body)
            if (obstacles.find(seg.first) != obstacles.end())
                snakes.erase(snake);

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

Snake::Snake(Coord head) {
    Coord cur = head;
    for (int i = 0; i < 5; i++){
        body.push_back(std::pair<Coord, dir>(cur, dir::RIGHT));
        cur.y--;
    }
}
