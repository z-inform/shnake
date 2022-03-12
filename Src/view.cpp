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
