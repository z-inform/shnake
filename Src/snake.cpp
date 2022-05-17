#include "textview.hpp"
#include "model.hpp"
#include "controller.hpp"
#include <stdio.h>
#include <memory>

int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Incorrect number of arguments\n");
        return -1;
    }
    std::shared_ptr<View> view = View::get(argv[1]);
    Game model;
    std::vector<std::string> but = {"d", "w", "a", "s"};
    Human human1(but, &model.snakes.front());
    Human human2({"\e[C", "\e[A", "\e[D", "\e[B"}, &model.snakes.back());
    view -> run();
    return 0;
}   
