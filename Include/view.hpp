#pragma once

#include <iostream>
#include <memory>

class View {

    public:
        static std::shared_ptr<View> get(const std::string arg);
        static std::shared_ptr<View> obj;
        virtual void draw() = 0;
        virtual void run () = 0;
};
