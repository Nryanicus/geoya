#ifndef utilities_hpp
#define utilities_hpp

#include <SFML/Graphics.hpp>
// #include <Box2D/Box2D.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "constants.hpp"

class Vector
{
public:
    float x,y;
    Vector() : x(0), y(0) {}
    Vector(float x, float y) : x(x), y(y) {}
    Vector(double x, double y) : x(x), y(y) {}
    Vector(int x, int y) : x((float)x), y((float)y) {}
    Vector(sf::Vector2f v) : x(v.x), y(v.y) {}
    Vector(sf::Vector2i v) : x((float)v.x), y((float)v.y) {}
    Vector operator-(Vector o)
    {
        return Vector(x-o.x, y-o.y);
    }
    Vector operator-=(Vector o)
    {
        x -= o.x;
        y -= o.y;
        return *this;
    }
    Vector operator+(Vector o)
    {
        return Vector(x+o.x, y+o.y);
    }
    Vector operator+=(Vector o)
    {
        x += o.x;
        y += o.y;
        return *this;
    }
    Vector operator*(float m)
    {
        return Vector(m*x, m*y);
    }
    Vector operator*=(float m)
    {
        x *= m;
        y *= m;
        return *this;
    }    
    Vector operator/(float m)
    {
        return Vector(x/m, y/m);
    }
    Vector operator/=(float m)
    {
        x /= m;
        y /= m;
        return *this;
    }
    float magnitude()
    {
        return sqrt(x*x + y*y);
    }
    float magnitude_squared()
    {
        return x*x + y*y;
    }
    Vector normalise()
    {
        float m = magnitude();
        return (*this)/m;
    }
    float dot(Vector other)
    {
        return x*other.x + y*other.y;
    }
    Vector projection(Vector other)
    {
        return other * dot(other)/other.dot(other);
    }

};

std::ostream& operator<<(std::ostream& os, const Vector& v);

//////////////////////
/*      Hex Ops     */
//////////////////////

std::pair<int, int> round_hex(float q, float r);

std::pair<int, int> pixel_to_axial(float x, float y);

std::pair<float, float> axial_to_pixel(float q, float r);

//////////////////////
/* SFML Vector Ops */
//////////////////////

sf::Vector2f rotate(float x, float y, float theta);

float magnitude(sf::Vector2f* v);

sf::Vector2f normalise(sf::Vector2f* v);

float dot_product(sf::Vector2f* v1, sf::Vector2f* v2);

// in degrees
float angle_between(sf::Vector2f* v1, sf::Vector2f* v2);

sf::RectangleShape line_with_thickness(sf::Vector2f p1, sf::Vector2f p2, float thickness, int trans1=255, int trans2=255);

//////////////////////
/* Box2D Vector Ops */
//////////////////////

// b2Vec2 rotate(b2Vec2* vec, float theta);

// float magnitude(b2Vec2* v);

// b2Vec2 normalise(b2Vec2* v);

// b2Vec2  project(b2Vec2* v1, b2Vec2* v2);

// // in radians
// float angle_between(b2Vec2* v1, b2Vec2* v2);


//////////////////////
/*     Misc Ops     */
//////////////////////

int random_int(int a, int b);

#endif