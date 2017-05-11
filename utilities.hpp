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

static std::string gesture_to_sprite(Gesture s, bool right=false)
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
    if (s == Gesture::Vert && right)
        return "Up";
    if (s == Gesture::Vert && !right)
        return "Down";
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

static std::string d2s(DIRECTION s)
{
    if (s == DIRECTION::UP)
        return "Up";
    if (s == DIRECTION::DOWN)
        return "Down";
    if (s == DIRECTION::RIGHT)
        return "Right";
    if (s == DIRECTION::LEFT)
        return "Left";
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
    InterpType type;

    Interpolation(){}
    Interpolation(T s, T e, double t, InterpType ty=InterpType::Linear)
    : start(s), end(e), time(t), type(ty), time_elapsed(0)
    {}
    bool update(double dt)
    {
        time_elapsed += dt;
        return time_elapsed >= time;
    }

    double get_bezier_ratio(double t, Vector p1, Vector p2)
    {
        if (t < 0.001) t = 0;
        Vector v = BEZIERSTART*pow(1-t, 3) + p1*3*pow(1-t,2)*t + p2*3*(1-t)*pow(t,2) + BEZIEREND*pow(t,3);
        return v.y;
    }

    T get_point()
    {
        double r;
        double t = time_elapsed/time;
        if (type == InterpType::Linear)
            r = t;
        else if (type == InterpType::CubicBezier)
            r = get_bezier_ratio(t, CUBICBEZIER1, CUBICBEZIER2);
        else if (type == InterpType::SineBezier)
            r = get_bezier_ratio(t, SINEBEZIER1, SINEBEZIER2);
        else
            assert(false);
        if(r > 1) r = 1;
        if(r < 0) r = 0;
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

sf::IntRect spritesheet_rect(std::string frame);

sf::Vector2f spritesheet_offset(std::string frame);

namespace std{
    bool isfinite(Vector v);
}

static Vector GESTURE_INDEXDIR_TO_POINT(Gesture g, IndexDirection id, int* position_index)
{
    Vector v;
    if (g == Gesture::Earth || g == Gesture::Death)
    {
        (*position_index) = EARTHPOINTTRANSLATE.at(id); 
        v = EARTHPOINTS.at(EARTHPOINTTRANSLATE.at(id));
    }
    else if (g == Gesture::Air)
    {
        (*position_index) = AIRPOINTTRANSLATE.at(id); 
        v = AIRPOINTS.at(AIRPOINTTRANSLATE.at(id));
    }
    else if (g == Gesture::Fire)
    {
        (*position_index) = FIREPOINTTRANSLATE.at(id); 
        v = FIREPOINTS.at(FIREPOINTTRANSLATE.at(id));
    }
    else if (g == Gesture::Water)
    {
        (*position_index) = WATERPOINTTRANSLATE.at(id); 
        v = WATERPOINTS.at(WATERPOINTTRANSLATE.at(id));
    }
    else if (g == Gesture::Life)
    {
        (*position_index) = LIFEPOINTTRANSLATE.at(id); 
        v = LIFEPOINTS.at(LIFEPOINTTRANSLATE.at(id));
    }
    else 
        assert(false);
    return v;
}

#endif