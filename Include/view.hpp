#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include "model.hpp"

class View {

    protected: 
        Coord max_coord;

    public:
        static std::shared_ptr<View> get(const std::string arg = "text");
        static std::shared_ptr<View> obj;
        virtual void draw() = 0;
        virtual void run () = 0;
        virtual void draw(Coord& rabbit) = 0; //draw a  rabbit 
        virtual void draw(Snake& snake) = 0; //draw a  rabbit 
        Coord get_max_coord();
        void set_max_coord(int x, int y);
        std::function<void(void)> draw_all;
        std::function<void(void)> game_tick;
};

