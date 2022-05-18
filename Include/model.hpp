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

    friend bool operator== (const Coord& l, const Coord& r) {
        return (l.x == r.x) && (l.y == r.y);
    }


    public:
        Coord(): x(0), y(0) {};
        Coord(int x, int y): x(x), y(y) {};
        int x;
        int y;
};

class Rabbit {

    friend bool operator< (const Rabbit& l, const Rabbit& r) {
        return std::tie(l.place, l.left_to_live) < std::tie(r.place, r.left_to_live);
    }

    public:
    Coord place;
    int left_to_live = 5;

    Rabbit(Coord coord, int life): place(coord), left_to_live(life){};
};

class Snake {
    public:

    Snake(Coord head);
    Snake() {};
    bool slowed = false;
    int miss_tick = 0;
    
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

    unsigned short num_rabbits = 50;

    std::set<Rabbit> rabbits;
    std::set<Coord> swamp;
    std::set<Coord> available;
    Coord get_rand_coord();

    std::mt19937 generator {static_cast<long unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())};

    void add_rabbit();

    void snakes_check_crash();
    void update_available();
    void grow_swamp();

    public:

    Game();
    std::list<Snake> snakes;
    void draw_all();
    void snakes_step();

};

