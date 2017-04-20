#ifndef glyph_drawer_hpp
#define glyph_drawer_hpp

#include <cstdlib>
#include <string>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include "Hand.hpp"
#include "constants.hpp"
#include "utilities.hpp"

class Hand;

const Vector RIGHTHAND_POS(50, 0);
const Vector LEFTHAND_POS(-50, 0);
const Vector GLYPH_POS(0, -250);
const double GLYPH_SCALE = 100.0;
const double GLYPH_THICKNESS = 5.0;

class HandData
{
public:
    Vector current;
    Vector start;
    Vector end;
    double time;
    double time_elapsed;
    Vector base_pos;
    std::vector<Vector> glyph_path; 
    sf::RenderTexture glyph_outline; 
    // sf::RenderTexture glyph; 
};

class GlyphDrawer
{
private:
    Hand* left_hand;
    Hand* right_hand;
    HandData left_hand_data;
    HandData right_hand_data;
    std::unordered_map<Hand*, HandData*> hand_data;

    Vector position;

    sf::Sprite draw_sprite;

public:

    GlyphDrawer(sf::Texture* top_hand_tex);

    ~GlyphDrawer();

    void notify_windup(Hand* hand);
    void notify_hold(Hand* hand);
    void notify_cancel(Hand* hand);
    void notify_winddown(Hand* hand);

    void draw(sf::RenderTarget* target);

    void update(double dt);

    void input(bool left, int finger, bool in);
};


#endif