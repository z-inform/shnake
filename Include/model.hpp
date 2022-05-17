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
    Snake() {};
    
    enum class dir {
        UP,
        LEFT,
        DOWN,
        RIGHT
    };

    std::list<std::pair<Coord, dir>> body;
    dir last_dir;
};

class Game {

    unsigned short num_rabbits = 500;

    std::set<Coord> rabbits;
    std::set<Coord> obstacles;
    std::set<Coord> available;
    Coord get_rand_coord();

    std::mt19937 generator {static_cast<long unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

    void add_rabbit();

    void snakes_check_crash();
    void update_available();

    public:

    Game();
    std::list<Snake> snakes;
    void draw_all();
    void snakes_step();

};

