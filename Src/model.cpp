#include <iostream>
#include <random>

#include "model.hpp"
#include "view.hpp"

Coord Game::get_rand_coord(){
    Coord place = View::get() -> get_max_coord();

    std::uniform_int_distribution<int> distrib_x (2, place.x - 2);
    std::uniform_int_distribution<int> distrib_y (2, place.y - 1);

    place.x = distrib_x(Game::generator);
    place.y = distrib_y(Game::generator);

    return place;
}

void Game::add_rabbit(){
    rabbits.insert(get_rand_coord());
}

Game::Game(){
    auto v = View::get();
    v -> draw_all = std::bind(&Game::draw_all, this);
    v -> game_tick = std::bind(&Game::snakes_step, this);

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

        for (auto s : snake.body)
            obstacles.insert(s.first);
    }

    //snakes_check_crash();
}

void Game::snakes_check_crash(){
    for (std::list<Snake>::iterator it = snakes.begin(); it != snakes.end(); it++) {
        if (obstacles.find(it -> body.front().first) != obstacles.end())
            snakes.erase(it);
    }
}

Snake::Snake(Coord head) {
    Coord cur = head;
    for (int i = 0; i < 5; i++){
        body.push_back(std::pair<Coord, dir>(cur, dir::RIGHT));
        cur.y--;
    }
}
