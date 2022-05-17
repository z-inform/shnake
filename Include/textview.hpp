#pragma once

#include "view.hpp"
#include "model.hpp"
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <functional>

class Textview : public View {

    public:

    virtual void run() override;
    Textview();
    ~Textview();
    static std::function<void(int)> c_sigint_handler;
    static std::function<void(int)> c_size_change_handler;

    private: 
    
    bool game_running;
    struct termios old_term_state;

    void hline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem);
    void vline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem);
    void screen_clear();
    void draw_frame();
    void sigint_handler(int sig);
    void size_change_handler(int sig);

    void draw(Coord& rabbit);
    void draw(Snake& snake);
};
