#pragma once

#include "view.hpp"
#include <termios.h>
#include <unistd.h>
#include <functional>

class Textview : public View {

    public:

    virtual void draw() override;
    virtual void run() override;
    Textview();
    ~Textview();
    static std::function<void(int)> f;

    private: 
    
    bool game_running;
    struct termios old_term_state;

    void hline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem);
    void vline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem);
    void screen_clear(unsigned int rows);
    void sigint_handler(int sig);
};