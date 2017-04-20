#ifndef ROUNDENDEDLINE_H
#define ROUNDENDEDLINE_H

// Class written by Foaly, modified by Nathmate
// https://github.com/SFML/SFML/wiki/Source:-Round-Ended-Lines

#include <SFML/Graphics/Shape.hpp>
#include <cmath>


class RoundedLine : public sf::Shape
{
public:

    RoundedLine(const sf::Vector2f& startPoint = sf::Vector2f(0, 0), const sf::Vector2f& endPoint = sf::Vector2f(0, 0), const float width = 1.0, const sf::Color colour=sf::Color::White, const float outlineWidth = 0.0, const sf::Color outlineColour=sf::Color::White);

    RoundedLine(const sf::Vector2f& endPoint = sf::Vector2f(0, 0), const float width = 1.0);

    void setEndPoint(const sf::Vector2f& endPoint);

    void setWidth(const float width);

    virtual std::size_t getPointCount() const;

    virtual sf::Vector2f getPoint(std::size_t index) const;

private :

    sf::Vector2f m_endPoint;
    float m_Width;
};

#endif //ROUNDENDEDLINE_H