#include "textview.hpp"
#include <termios.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <functional>

std::function<void(int)> Textview::c_sigint_handler;
std::function<void(int)> Textview::c_size_change_handler;

void sigint_handler(int sig){
    Textview::c_sigint_handler(sig);
}

void size_change_handler(int sig){
    Textview::c_size_change_handler(sig); 
}

void Textview::sigint_handler(int sig){
    Textview::game_running = false;
}

void Textview::size_change_handler(int sig){
    Textview::screen_clear(Textview::win_size.ws_row);
    Textview::draw();
}

Textview::Textview(){
    std::cout << "oaoa text constructor" << std::endl;

    tcgetattr(0, &old_term_state);
    struct termios raw;
    cfmakeraw(&raw);
    raw.c_lflag |= ISIG;
    raw.c_cc[VINTR] = '';
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size); 
    tcsetattr(0, TCSANOW, &raw);

    c_sigint_handler = std::bind(&Textview::sigint_handler, this, std::placeholders::_1);
    c_size_change_handler = std::bind(&Textview::size_change_handler, this, std::placeholders::_1);
    struct sigaction act;
    act.sa_handler = &::sigint_handler;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    act.sa_handler = &::size_change_handler;
    sigaction(SIGWINCH, &act, NULL);
}

Textview::~Textview(){
    std::cout << std::endl;
    tcsetattr(0, TCSANOW, &old_term_state);
}

void Textview::draw(){
    
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size); 

    screen_clear(win_size.ws_row);
    hline(1, 1, win_size.ws_col, "\e[33m-");
    vline(2, 1, win_size.ws_row - 2, "\e[33m#");
    vline(2, win_size.ws_col, win_size.ws_row - 2, "\e[33m#");
    hline(win_size.ws_row - 1, 1, win_size.ws_col, "\e[33m-");

    fflush(stdout);
}

void Textview::run(){
    struct pollfd input = {0, POLL_IN, 0};
    screen_clear(win_size.ws_row);
    draw();
    game_running = true;
    while (game_running) {
        
        if (poll(&input, 1, 1) == 1){
            char inc_char;
            read(0, &inc_char, 1);

            if (inc_char == 'q')
                game_running = false;
        }

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

