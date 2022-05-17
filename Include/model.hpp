#pragma once

#include <iostream>
#include <random>
#include <chrono>
#include <list>
#include <set>
#include <tuple>

class Coord {

    friend bool operator< (const Coord& l, const Coord& r) {
        return std::tie(l.x, l.y) < std::tie(r.x, r.y);
    }

    public:
        int x;
        int y;
};

class Snake {
    public:

    Snake(Coord head);
    
    enum class dir {
        UP,
        LEFT,
        DOWN,
        RIGHT
    };

    std::list<std::pair<Coord, dir>> body;
};

class Game {

    unsigned short num_rabbits = 50;

    std::set<Coord> rabbits;
    std::set<Coord> obstacles;
    Coord get_rand_coord();

    std::mt19937 generator {static_cast<long unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

    void add_rabbit();
    std::list<Snake> snakes;

    void snakes_check_crash();

    public:

    Game();
    void draw_all();
    void snakes_step();

};

