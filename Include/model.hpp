#pragma once

#include <iostream>
#include <random>
#include <chrono>
#include <list>
#include "view.hpp"

class Game {

    std::list<Coord> rabbits;
    Coord get_rand_coord();

    std::mt19937 generator {static_cast<long unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

};

