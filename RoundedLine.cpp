#include "RoundedLine.hpp"

RoundedLine::RoundedLine(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, const float width, const sf::Color colour, const float outlineWidth, const sf::Color outlineColour)
  : m_endPoint (endPoint), m_Width (width)
{
    setPosition(startPoint);
    setOutlineThickness(outlineWidth);
    setFillColor(colour);
    setOutlineColor(outlineColour);
    update();
}

RoundedLine::RoundedLine(const sf::Vector2f& endPoint, const float width)
  : m_endPoint (endPoint), m_Width (width)
{
    update();
}

void RoundedLine::setEndPoint(const sf::Vector2f& endPoint)
{
    m_endPoint = endPoint;
    update();
}

void RoundedLine::setWidth(const float width)
{
    m_Width = width;
    update();
}

std::size_t RoundedLine::getPointCount() const
{
    return 30;
}

sf::Vector2f RoundedLine::getPoint(std::size_t index) const
{
    sf::Vector2f P1(1.0, 0.0);
    sf::Vector2f P2(m_endPoint + sf::Vector2f(1.0, 0.0) - getPosition());

    sf::Vector2f offset;
    int iFlipDirection;

    if(index < 15)
    {
        offset = P2;
        iFlipDirection = 1;
    }
    else
    {
        offset = P1;
        iFlipDirection = -1;
        index -= 15;
    }

    float start = -atan2(P1.y - P2.y, P2.x - P1.x);

    float angle = index * M_PI / 14 - M_PI / 2 + start;
    float x = std::cos(angle) * m_Width / 2;
    float y = std::sin(angle) * m_Width / 2;

    return sf::Vector2f(offset.x + x * iFlipDirection, offset.y + y * iFlipDirection);
}