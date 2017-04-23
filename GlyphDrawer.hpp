#ifndef glyph_drawer_hpp
#define glyph_drawer_hpp

#include <cstdlib>
#include <cmath>
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

const Vector BASE_POS(WIDTH/2, HEIGHT-500);
const Vector RIGHTHAND_POS(500, 0);
const Vector LEFTHAND_POS(-500, 0);
const Vector GLYPH_POS(0, -250);
const double GLYPH_SCALE = 100.0;
const double GLYPH_MOVEMENT_SCALE = 100.0;
const double GLYPH_THICKNESS = 10.0;

class HandData
{
public:
    // for wandering
    Vector current;
    Vector start;
    Vector end;
    double time;
    double time_elapsed;

    // for drawing
    Vector base_pos;
    std::vector<Vector> glyph_path; 
    std::vector<Vector> move_path; 
    sf::RenderTexture glyph_outline;
    bool prev_valid;
    Vector prev_glyph_draw_point;
};

class GlyphDrawer
{
private:
    Hand* left_hand;
    Hand* right_hand;
    HandData left_hand_data;
    HandData right_hand_data;
    std::unordered_map<Hand*, HandData*> hand_data;
    std::unordered_map<Hand*, Hand*> other_hand;

    Vector position;

    sf::Sprite draw_sprite;
    sf::RenderTexture glyph;

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