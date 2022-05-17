#include "textview.hpp"
#include <termios.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <functional>
#include <chrono>

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

    tcgetattr(0, &old_term_state);
    struct termios raw;
    cfmakeraw(&raw);
    raw.c_lflag |= ISIG;
    raw.c_cc[VINTR] = '';
    struct winsize win_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size); 
    max_coord.x = win_size.ws_row;
    max_coord.y = win_size.ws_col / 2;
    min_coord.x = 2;
    min_coord.y = 2;
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

void Textview::draw(Coord& rabbit){
    printf("\e[%d;%dH", rabbit.x, rabbit.y * 2 - 1);
    printf("\e[96m❂ ");
    fflush(stdout);
}

void Textview::draw(Snake& snake){

    for (auto segment : snake.body) {
        printf("\e[%d;%dH", segment.first.x, segment.first.y * 2 - 1);    

        switch (segment.second) {
            case Snake::dir::RIGHT:
                printf("\e[95m⇛ ");
                break;
            case Snake::dir::UP:
                printf("\e[95m⤊ ");
                break;
            case Snake::dir::LEFT:
                printf("\e[95m⇚ ");
                break;
            case Snake::dir::DOWN:
                printf("\e[95m⤋ ");
                break;
        }
    }
    fflush(stdout);
}

void Textview::run(){
    struct pollfd input = {0, POLL_IN, 0};
    screen_clear();
    draw_frame();
    game_running = true;
    auto delay_start = std::chrono::steady_clock::now();

    while (game_running) {
    
        std::string req = ""; 
        while (poll(&input, 1, 1) == 1){
            char inc_char;
            read(0, &inc_char, 1);
            
            if (inc_char == 'q') {
                game_running = false;
                break;
            }

            req += inc_char;
            auto res = button_table.find(req);
            if (res != button_table.end()) {
                res -> second();
                req = "";
            }
        }

        screen_clear();
        draw_frame();
        draw_all();

        if (std::chrono::steady_clock::now() - delay_start > tick_time) {
            game_tick();
            delay_start = std::chrono::steady_clock::now();
        }

        usleep(100000);

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
    printf("\e[1;1H\e[J");
}

