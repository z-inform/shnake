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
    rabbits.push_back(get_rand_coord());
}

Game::Game(){
    auto v = View::get();
    v -> draw_all = std::bind(&Game::draw_all, this);

    for (int i = 0; i < num_rabbits; i++){
        add_rabbit();
    }
}

void Game::draw_all(){
    auto v = View::get();

    for (auto r : rabbits)
        v -> draw(r);

}
