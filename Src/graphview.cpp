#include "graphview.hpp"
#include <iostream>

Graphview::Graphview(){
    std::cout << "oaoa graph constructor" << std::endl;
}

Graphview::~Graphview(){

}

void Graphview::draw(){
    std::cout << "oaoa graph draw" << std::endl;
}

void Graphview::run(){
    std::cout << "oaoa graph run" << std::endl;
    bool end_run = false;
    while (!end_run) {
        end_run = true;
    }
}
