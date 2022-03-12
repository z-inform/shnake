#include "textview.hpp"
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <functional>

std::function<void(int)> Textview::f;

void sigint_handler(int sig){
    Textview::f(sig);
}

void Textview::sigint_handler(int sig){
    Textview::game_running = false;
}

Textview::Textview(){
    std::cout << "oaoa text constructor" << std::endl;

    tcgetattr(0, &old_term_state);
    struct termios raw;
    cfmakeraw(&raw);
    raw.c_lflag |= ISIG;
    raw.c_cc[VINTR] = '';
    tcsetattr(0, TCSANOW, &raw);

    f = std::bind(&Textview::sigint_handler, this, std::placeholders::_1);
    struct sigaction act;
    act.sa_handler = &::sigint_handler;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
}

Textview::~Textview(){
    std::cout << "text ded" << std::endl;
    tcsetattr(0, TCSANOW, &old_term_state);
}

void Textview::draw(){
    screen_clear(80);
    hline(1, 1, 40, "\e[33m-");
    vline(2, 1, 40, "\e[33m#");
    vline(2, 40, 40, "\e[33m#");
    hline(41, 1, 40, "\e[33m-");
    fflush(stdout);
}

void Textview::run(){
    std::cout << "oaoa text run" << std::endl;
    game_running = true;
    while (game_running) {
        draw();
        usleep(1000);
    }
}

void Textview::hline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem){
    printf("\e[%d;%dH", x, y);
    for(unsigned int i = 0; i < length; i++){
        std::cout << elem;
    }
    std::cout << "\e[m";
}

void Textview::vline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem){
    printf("\e[%d;%dH", x, y);
    for(unsigned int i = 0; i < length; i++){
        std::cout << elem;
        printf("\e[%d;%dH", x + i, y);
    }
    std::cout << "\e[m";
}

void Textview::screen_clear(unsigned int rows){
    printf("\e[1;1H");
    for(unsigned int i = 1; i <= rows; i++){
        std::cout << "\e[J";
        printf("\e[%d;1H", i);
    }
}

