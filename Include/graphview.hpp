#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <set>

#include "view.hpp"
#include "model.hpp"

class Graphview : public View {

    int cell_size = 16;
    bool game_running;
    sf::RenderWindow window;
    sf::Texture texture;

    public:
    virtual void run() override;
    Graphview();
    ~Graphview();

    void draw_frame();
    void draw(Rabbit& rabbit);
    void draw(Snake& snake);
    void draw(std::set<Coord>& swamp);
    std::string key_to_str(sf::Keyboard::Key code);

};
