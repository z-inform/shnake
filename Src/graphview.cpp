#include "graphview.hpp"
#include "model.hpp"
#include <iostream>
#include <chrono>

Graphview::Graphview():
    window(sf::VideoMode((100 + 1) * cell_size, (60 + 1) * cell_size), "OAOA MMMM", sf::Style::Titlebar | sf::Style::Close ) {
    max_coord = {60, 100};
    min_coord = {1, 1};
    window.setVerticalSyncEnabled(true);
    texture.loadFromFile("sprites/sheet.png");
}

Graphview::~Graphview(){

}

void Graphview::draw(Coord& rabbit){
    sf::IntRect rab_rect(32, 48, 16, 16);
    sf::Sprite sprite(texture, rab_rect);
    sprite.setPosition(rabbit.y * cell_size, rabbit.x * cell_size);
    window.draw(sprite);
}

void Graphview::draw(Snake& snake){
    sf::IntRect seg_rect;
    seg_rect.width = 16;
    seg_rect.height = 16;
    sf::Sprite pic(texture);
    for (auto segment = snake.body.begin(); segment != snake.body.end(); segment++) {
        if (segment == snake.body.begin()) {
            seg_rect.top = 0;
            switch (segment -> second) {
                case Snake::dir::UP:
                    seg_rect.left = 0;
                    break;
                case Snake::dir::RIGHT:
                    seg_rect.left = 16;
                    break;
                case Snake::dir::LEFT:
                    seg_rect.left = 48;
                    break;
                case Snake::dir::DOWN:
                    seg_rect.left = 32;
                    break;
            }
        } else if (segment == std::prev(snake.body.end())) {
            seg_rect.top = 16;
            switch (segment -> second) {
                case Snake::dir::UP:
                    seg_rect.left = 0;
                    break;
                case Snake::dir::RIGHT:
                    seg_rect.left = 16;
                    break;
                case Snake::dir::LEFT:
                    seg_rect.left = 48;
                    break;
                case Snake::dir::DOWN:
                    seg_rect.left = 32;
                    break;
            }
        }
        else if (std::next(segment) -> second == segment -> second) {
            if ((segment -> second == Snake::dir::UP) || (segment -> second == Snake::dir::DOWN)) {
                seg_rect.left = 0;
                seg_rect.top = 48;
            } else {
                seg_rect.left = 16;
                seg_rect.top = 48;
            }
        } else {
            seg_rect.top = 32;
            switch (segment -> second) {
                case Snake::dir::UP:
                    if (std::next(segment) -> second == Snake::dir::RIGHT)
                        seg_rect.left = 48;
                    else 
                        seg_rect.left = 0;
                    break;
                case Snake::dir::RIGHT:
                    if (std::next(segment) -> second == Snake::dir::DOWN)
                        seg_rect.left = 0;
                    else 
                        seg_rect.left = 16;
                    break;
                case Snake::dir::LEFT:
                    if (std::next(segment) -> second == Snake::dir::DOWN)
                        seg_rect.left = 48;
                    else 
                        seg_rect.left = 32;
                    break;
                case Snake::dir::DOWN:
                    if (std::next(segment) -> second == Snake::dir::RIGHT)
                        seg_rect.left = 32;
                    else 
                        seg_rect.left = 16;
                    break;
            }
        }

        pic.setTextureRect(seg_rect);
        pic.setPosition(segment -> first.y * cell_size, segment -> first.x * cell_size);
        window.draw(pic);
    }
}

void Graphview::run(){
    auto delay_start = std::chrono::steady_clock::now();
	while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case (sf::Keyboard::Q):
                        window.close();
                        break;
                    default:
                        auto res = button_table.find(key_to_str(event.key.code));
                        if (res != button_table.end())
                            res -> second();
                        break;
                }
            }
		}

    if (std::chrono::steady_clock::now() - delay_start > tick_time) {
        game_tick();
        delay_start = std::chrono::steady_clock::now();
    }
    window.clear();
    draw_frame();
    draw_all();
    window.display();
	}
}

void Graphview::draw_frame(){
    sf::RectangleShape frame;
    frame.setSize(sf::Vector2f((max_coord.y - 1) * cell_size, (max_coord.x - 1) * cell_size));
    frame.setPosition(cell_size, cell_size);
    frame.setOutlineColor(sf::Color(0, 111, 184));
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(cell_size);
    window.draw(frame);
}

std::string Graphview::key_to_str(sf::Keyboard::Key code){
    switch (code) {
        case sf::Keyboard::D:
            return "d";
        case sf::Keyboard::W:
            return "w";
        case sf::Keyboard::A:
            return "a";
        case sf::Keyboard::S:
            return "s";
        case sf::Keyboard::Right:
            return "\e[C";
        case sf::Keyboard::Up:
            return "\e[A";
        case sf::Keyboard::Left:
            return "\e[D";
        case sf::Keyboard::Down:
            return "\e[B";
        default: 
            return "";
    }
}
