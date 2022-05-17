#pragma once

#include <string>
#include <vector>

#include "model.hpp"


class Human : public Snake {
    Snake* controlled_snake;
    std::vector<std::string> buttons_;


    public: 
    void button_handler(std::string button);
    Human(const std::vector<std::string> &buttons, Snake* my_snake);
};
