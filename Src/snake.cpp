#include "textview.hpp"
#include "model.hpp"
#include <stdio.h>
#include <memory>

int main(){
    printf("Not a game (for now)\n");
    std::shared_ptr<View> view = View::get("text");
    Game model;
    view -> run();

    return 0;
}   
