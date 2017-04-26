#ifndef hand_hpp
#define hand_hpp

#include <cstdlib>
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "GlyphDrawer.hpp"
#include "RoundedLine.hpp"
#include "constants.hpp"
#include "utilities.hpp"


class GlyphDrawer;

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