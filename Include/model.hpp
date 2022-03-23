#pragma once

#include <iostream>
#include <random>
#include <chrono>
#include <list>
#include "view.hpp"

class Game {

    unsigned short num_rabbits = 50;

    std::list<Coord> rabbits;
    Coord get_rand_coord();

    std::mt19937 generator {static_cast<long unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

    void add_rabbit();

    public:

    Game();
    void draw_all();

};

