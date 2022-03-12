#include "textview.hpp"
#include <stdio.h>
#include <memory>

int main(){
    printf("Not a game (for now)\n");
    std::shared_ptr<View> view = View::get("text");
    view -> run();

    return 0;
}   
