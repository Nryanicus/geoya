#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "GlyphDrawer.hpp"
#include "constants.hpp"
#include "utilities.hpp"

int main(int argc, char* argv[])
{   
    if (argc != 1)
    {
        printf("Usage: ./main\n");
        return -1;
    }

    // create the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Gestures", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::Clock clock;
    float time = 0;

    sf::Font font;
    font.loadFromFile("LiberationMono-Regular.ttf");

    sf::Texture top_hand_tex;
    top_hand_tex.loadFromFile("BlenderRender/Sprites/TopSpriteSheet.png");

    GlyphDrawer glyph_drawer(&top_hand_tex);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();

                if (event.key.code == sf::Keyboard::LShift)
                    glyph_drawer.input(true, 4, true);
                if (event.key.code == sf::Keyboard::A)
                    glyph_drawer.input(true, 3, true);
                if (event.key.code == sf::Keyboard::S)
                    glyph_drawer.input(true, 2, true);
                if (event.key.code == sf::Keyboard::D)
                    glyph_drawer.input(true, 1, true);
                if (event.key.code == sf::Keyboard::Space)
                    glyph_drawer.input(true, 0, true);
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::LShift)
                    glyph_drawer.input(true, 4, false);
                if (event.key.code == sf::Keyboard::A)
                    glyph_drawer.input(true, 3, false);
                if (event.key.code == sf::Keyboard::S)
                    glyph_drawer.input(true, 2, false);
                if (event.key.code == sf::Keyboard::D)
                    glyph_drawer.input(true, 1, false);
                if (event.key.code == sf::Keyboard::Space)
                    glyph_drawer.input(true, 0, false);
            }
        }
        time = clock.restart().asSeconds();
        glyph_drawer.update(time);

        window.clear(Grey);
        glyph_drawer.draw(&window);
        window.display();

    }
    return 0;
}
