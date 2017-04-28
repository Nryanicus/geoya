#ifndef glyph_drawer_hpp
#define glyph_drawer_hpp

#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include "Glyph.hpp"
#include "Hand.hpp"
#include "constants.hpp"
#include "utilities.hpp"

class Hand;

static std::string hs2s(HandDataState s)
{
    if (s == HandDataState::None)
        return "None";
    if (s == HandDataState::Wandering)
        return "Wandering";
    if (s == HandDataState::Holding)
        return "Holding";
    if (s == HandDataState::Moving)
        return "Moving";
    if (s == HandDataState::Drawing)
        return "Drawing";
    if (s == HandDataState::Modifying)
        return "Modifying";
    assert(false);
    return "";
}

class HandData
{
public:

    HandData()
    : state(HandDataState::None), glyph_outline(NULL)
    {}

    // take on glyph other is currently drawing
    void swap(HandData* other, Gesture gesture)
    {
        state = HandDataState::Drawing;
        current_glyph = new Glyph(gesture);
        current_glyph->scale = other->current_glyph->scale_interp.get_point();
        current_glyph->scale_active = true;
        current_glyph->scale_interp = other->current_glyph->scale_interp;
        current_glyph->reverse_scale();
        
        glyph_outline = new GlyphOutline(gesture);
        glyph_outline->scale = other->glyph_outline->scale_interp.get_point();
        glyph_outline->scale_active = true;
        glyph_outline->scale_interp = other->glyph_outline->scale_interp;
        glyph_outline->reverse_scale();
    }

    ~HandData()
    {
        if (glyph_outline)
            delete glyph_outline;
    }

    HandDataState state;

    // for wandering
    Interp2D hand_translate;

    // for drawing
    Vector current;
    Vector base_pos;
    std::vector<Vector> move_path; 
    GlyphOutline* glyph_outline;
    Glyph* current_glyph;
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

    std::vector<Glyph*> glyphs;

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