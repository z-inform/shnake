#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <chrono>
#include <list>
#include <string>
#include <unordered_map>
#include "model.hpp"

using namespace std::chrono_literals;

class View {

    protected: 
        Coord max_coord;
        Coord min_coord;

    public:
        std::unordered_map<std::string, std::function<void(void)>> button_table;
        std::chrono::milliseconds tick_time = 500ms;
        static std::shared_ptr<View> get(const std::string arg = "text");
        static std::shared_ptr<View> obj;
        virtual void run () = 0;
        virtual void draw(Coord& rabbit) = 0; //draw a rabbit 
        virtual void draw(Snake& snake) = 0; //draw a snake
        Coord get_max_coord();
        Coord get_min_coord();
        void set_max_coord(int x, int y);
        std::function<void(void)> draw_all;
        std::function<void(void)> game_tick;
};

