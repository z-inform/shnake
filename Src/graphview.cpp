#include "graphview.hpp"
#include <iostream>

Graphview::Graphview():
    window(sf::VideoMode((100 + 1) * cell_size, (60 + 1) * cell_size), "OAOA MMMM", sf::Style::Titlebar | sf::Style::Close ) {
    max_coord = {100, 60};
    min_coord = {1, 1};
    window.setVerticalSyncEnabled(true);
    texture.loadFromFile("sprites/sheet.png");
}

Graphview::~Graphview(){

}

void Graphview::draw(Coord& rabbit){
    sf::IntRect rab_rect(32, 48, 16, 16);
    sf::Sprite sprite(texture, rab_rect);
    sprite.setPosition(rabbit.x * cell_size, rabbit.y * cell_size);
    window.draw(sprite);
}

void Graphview::draw(Snake& snake){

}

void Graphview::run(){
	while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
		}
    window.clear();
    draw_frame();
    draw_all();
    window.display();
	}
}

void Graphview::draw_frame(){
    sf::RectangleShape frame;
    frame.setSize(sf::Vector2f((max_coord.x - 1) * cell_size, (max_coord.y - 1) * cell_size));
    frame.setPosition(cell_size, cell_size);
    frame.setOutlineColor(sf::Color(0, 111, 184));
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(cell_size);
    window.draw(frame);
}
