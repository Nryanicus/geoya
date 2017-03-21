#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
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
            }
        }
        // clear the window with black color
        window.clear(DarkGrey);

        // end the current frame
        window.display();
        time = clock.restart().asSeconds();
    }
    return 0;
}
