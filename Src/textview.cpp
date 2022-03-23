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
    Textview::screen_clear();
    Textview::draw_frame();
}

Textview::Textview(){
    std::cout << "oaoa text constructor" << std::endl;

    tcgetattr(0, &old_term_state);
    struct termios raw;
    cfmakeraw(&raw);
    raw.c_lflag |= ISIG;
    raw.c_cc[VINTR] = '';
    struct winsize win_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size); 
    max_coord.x = win_size.ws_row + 1;
    max_coord.y = win_size.ws_col / 2;
    tcsetattr(0, TCSANOW, &raw);

    c_sigint_handler = std::bind(&Textview::sigint_handler, this, std::placeholders::_1);
    c_size_change_handler = std::bind(&Textview::size_change_handler, this, std::placeholders::_1);
    struct sigaction act;
    act.sa_handler = &::sigint_handler;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    act.sa_handler = &::size_change_handler;
    sigaction(SIGWINCH, &act, NULL);

    printf("\e[?25l");
}

Textview::~Textview(){
    screen_clear();
    tcsetattr(0, TCSANOW, &old_term_state);
    printf("\e[?25h");
}

void Textview::draw_frame(){
    
    struct winsize win_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size); 

    max_coord.x = win_size.ws_row + 1;
    max_coord.y = win_size.ws_col / 2;

    screen_clear();

    hline(1, 1, max_coord.y, "\e[34m⌘ ");
    vline(2, 1, max_coord.x, "\e[34m⌘");
    vline(2, max_coord.y, max_coord.x, "\e[34m⌘");
    hline(max_coord.x, 1, max_coord.y, "\e[34m⌘ ");

    fflush(stdout);
}

void Textview::draw(){



}

void Textview::draw(Coord& rabbit){
    printf("\e[%d;%dH", rabbit.x, rabbit.y * 2 - 1);
    printf("\e[96m❂ ");
}

void Textview::run(){
    struct pollfd input = {0, POLL_IN, 0};
    screen_clear();
    draw_frame();
    game_running = true;
    while (game_running) {
    
        draw_all();
        
        if (poll(&input, 1, 1) == 1){
            char inc_char;
            read(0, &inc_char, 1);

            if (inc_char == 'q')
                game_running = false;
        }

        usleep(10000);
    }
}

void Textview::hline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem){
    printf("\e[%d;%dH", x, y * 2 - 1);
    for(unsigned int i = 0; i < length; i++){
        std::cout << elem;
    }
    std::cout << "\e[m";
}

void Textview::vline(unsigned int x, unsigned int y, unsigned int length, const std::string& elem){
    printf("\e[%d;%dH", x, y * 2 - 1);
    for(unsigned int i = 0; i < length; i++){
        std::cout << elem;
        printf("\e[%d;%dH", x + i - 1, y * 2 - 1);
    }
    std::cout << "\e[m";
}

void Textview::screen_clear(){
    printf("\e[1;1H \e[J");
}

