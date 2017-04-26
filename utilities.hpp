#ifndef utilities_hpp
#define utilities_hpp

#include <SFML/Graphics.hpp>
// #include <Box2D/Box2D.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "Vector.hpp"
#include "constants.hpp"

static bool is_drawable(Gesture g)
{
    return DRAWABLE_GESTURE.find(g) != DRAWABLE_GESTURE.end();
}

static std::string gesture_to_sprite(Gesture s)
{
    if (s == Gesture::Air)
        return "Air";
    if (s == Gesture::Fire)
        return "Fire";
    if (s == Gesture::Earth)
        return "Earth";
    if (s == Gesture::Water)
        return "Water";
    if (s == Gesture::Life)
        return "Life";
    if (s == Gesture::Death)
        return "Death";
    if (s == Gesture::Vert)
        return "Vert";
    if (s == Gesture::Horz)
        return "Horz";
    if (s == Gesture::Cast)
        return "Cast";
    if (s == Gesture::Rotate)
        return "Rotate";
    assert(false);
    return "";
}

static std::string g2s(Gesture s)
{
    if (s == Gesture::Air)
        return "Air";
    if (s == Gesture::Fire)
        return "Fire";
    if (s == Gesture::Earth)
        return "Earth";
    if (s == Gesture::Water)
        return "Water";
    if (s == Gesture::Life)
        return "Life";
    if (s == Gesture::Death)
        return "Death";
    if (s == Gesture::Vert)
        return "Vert";
    if (s == Gesture::Horz)
        return "Horz";
    if (s == Gesture::Cast)
        return "Cast";
    if (s == Gesture::Rotate)
        return "Rotate";
    if (s == Gesture::None)
        return "None";
    assert(false);
    return "";
}

static std::string fs2s(FingerState s)
{
    if (s == FingerState::Opening)
        return "Opening";
    if (s == FingerState::Open)
        return "Open";
    if (s == FingerState::Closing)
        return "Closing";
    if (s == FingerState::Closed)
        return "Closed";
    assert(false);
    return "";
}

static std::string hs2s(HandState s)
{
    if (s == HandState::None)
        return "None";
    if (s == HandState::WindUp)
        return "WindUp";
    if (s == HandState::Hold)
        return "Hold";
    if (s == HandState::WindDown)
        return "WindDown";
    if (s == HandState::Cancel)
        return "Cancel";
    assert(false);
    return "";
}

template <class T> class Interpolation
{
public:
    T start;
    T end;
    double time;
    double time_elapsed;

    Interpolation(){}
    Interpolation(T s, T e, double t)
    : start(s), end(e), time(t)
    {}
    bool update(double dt)
    {
        time_elapsed += dt;
        return time_elapsed >= time;
    }
    T get_point()
    {
        double r = time_elapsed/time;
        return start*(1-r) + end*r;
    }
};

typedef Interpolation<double> Interp1D;
typedef Interpolation<Vector> Interp2D;

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

sf::IntRect spritesheet_rect(std::string sprite, std::string frame);

sf::Vector2f spritesheet_offset(std::string sprite, std::string frame);

#endif