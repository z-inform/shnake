#include "view.hpp"
#include "textview.hpp"
#include "graphview.hpp"
#include <memory>

std::shared_ptr<View> View::obj;

std::shared_ptr<View> View::get(const std::string arg){

    if (!obj) {

        if (arg == "graph") 
            obj = std::make_shared<Graphview>();

        if (arg == "text")
            obj = std::make_shared<Textview>();

    }
    
    return obj;
}

Coord View::get_max_coord(){
    return View::max_coord;
}

void View::set_max_coord(int x, int y){
    View::max_coord.x = x;
    View::max_coord.y = y;
}
