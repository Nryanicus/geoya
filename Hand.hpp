#ifndef hand_hpp
#define hand_hpp

#include <cstdlib>
#include <string>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "GlyphDrawer.hpp"
#include "constants.hpp"
#include "utilities.hpp"

class GlyphDrawer;

enum class FingerState {Opening, Open, Closing, Closed};
enum class HandState {None, WindUp, Hold, WindDown, Cancel};
enum class Gesture {None, Air, Fire, Earth, Water, Life, Death, Vert, Horz, Cast, Rotate};

const std::unordered_map<Gesture, std::vector<Vector>> GESTURE_TO_VERTICES = {
    {Gesture::Air, AIRLINES},
    {Gesture::Fire, FIRETRIANGLE},
    {Gesture::Earth, EARTHSQUARE},
    {Gesture::Water, WATERCIRCLE},
    {Gesture::Life, LIFESQUIGGLE},
    {Gesture::Death, DEATHCROSS},
};

const std::unordered_map<Gesture, sf::Color> GESTURE_TO_COLOUR = {
    {Gesture::Air, AIR_COLOUR},
    {Gesture::Fire, FIRE_COLOUR},
    {Gesture::Earth, EARTH_COLOUR},
    {Gesture::Water, WATER_COLOUR},
    {Gesture::Life, LIFE_COLOUR},
    {Gesture::Death, DEATH_COLOUR},
};

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

class Hand
{
public:
    GlyphDrawer* parent;

    bool right;
    std::string angle;

    std::vector<FingerState> finger_states;
    std::vector<double> finger_times;
    std::vector<int> finger_frames;

    Gesture gesture;
    HandState hand_state;
    int hand_frame;
    double hand_time;

    bool dirty;

    sf::RenderTexture canvas;
    sf::Sprite working;
    sf::Sprite draw_sprite;

    Vector position;

    Hand(sf::Texture* tex, GlyphDrawer* parent, std::string angle, bool right);

    void draw(sf::RenderTarget* target);

    void update(double dt);

    Gesture check_gesture();

    void input(int finger, bool in);
};

#endif