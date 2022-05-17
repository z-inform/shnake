#include <functional>

#include "controller.hpp"
#include "view.hpp"

Human::Human(const std::vector<std::string> &buttons, Snake* my_snake):
    controlled_snake(my_snake) {
    auto v = View::get();
    for (std::string s : buttons) {
        v -> button_table.insert({s, std::bind(&Human::button_handler, this, s)});
    }
    for (auto s : buttons) {
        buttons_.push_back(s);
    }
}

void Human::button_handler(std::string button){
        if (button == buttons_[0]) {
            if (controlled_snake -> last_dir != Snake::dir::LEFT)
                controlled_snake -> body.front().second = Snake::dir::RIGHT;
        }
        else if (button == buttons_[1]) {
            if (controlled_snake -> last_dir != Snake::dir::DOWN)
                controlled_snake -> body.front().second = Snake::dir::UP;
        }
        else if (button == buttons_[2]) {
            if (controlled_snake -> last_dir != Snake::dir::RIGHT)
                controlled_snake -> body.front().second = Snake::dir::LEFT;
        }
        else if (button == buttons_[3]) {
            if (controlled_snake -> last_dir != Snake::dir::UP)
                controlled_snake -> body.front().second = Snake::dir::DOWN;
        }
}
