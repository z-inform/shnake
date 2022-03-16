#include <iostream>
#include <random>

#include "model.hpp"
#include "view.hpp"

Coord Game::get_rand_coord(){
    Coord place = View::get() -> get_max_coord();

    std::uniform_int_distribution<int> distrib_x (1, place.x);
    std::uniform_int_distribution<int> distrib_y (1, place.y);

    place.x = distrib_x(Game::generator);
    place.y = distrib_y(Game::generator);

    return place;
}
