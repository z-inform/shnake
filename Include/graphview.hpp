#pragma once

#include "view.hpp"

class Graphview : public View {

    public:
    virtual void draw() override;
    virtual void run() override;
    Graphview();
    ~Graphview();

    private:
    void putc(unsigned int x, unsigned int y, unsigned int charcode);

};
