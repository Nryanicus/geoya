#include "GlyphDrawer.hpp"

GlyphDrawer::GlyphDrawer(sf::Texture* top_hand_tex, sf::Texture* earth_tex, sf::Texture* air_tex, sf::Texture* fire_tex, sf::Texture* water_tex, sf::Texture* life_tex, sf::Texture* death_tex)
: earth_tex(earth_tex), air_tex(air_tex), fire_tex(fire_tex), water_tex(water_tex), life_tex(life_tex), death_tex(death_tex)
{
    left_hand = new Hand(top_hand_tex, this, "TopBack", false);
    right_hand = new Hand(top_hand_tex, this, "TopBack", true);

    left_hand_data.base_pos = LEFTHAND_POS;
    right_hand_data.base_pos = RIGHTHAND_POS;

    hand_data[left_hand] = &left_hand_data;
    hand_data[right_hand] = &right_hand_data;
}

GlyphDrawer::~GlyphDrawer()
{
    delete left_hand;
    delete right_hand;
}

void GlyphDrawer::notify_windup(Hand* hand)
{
    hand_data[hand]->start = hand_data[hand]->current;
    hand_data[hand]->end = Vector(0.0, 0.0);
    // have hand travel at ~30pixels/sec(?) or in 0.41 (just short of the time to complete a WindUp anim), whichever is smaller
    hand_data[hand]->time = std::min( (hand_data[hand]->start-hand_data[hand]->end).magnitude()/30.0, 0.41);
    if (hand_data[hand]->time <= 0) hand_data[hand]->time = 0.1;
    hand_data[hand]->time_elapsed = 0;
}

void GlyphDrawer::notify_hold(Hand* hand)
{
    try
    {
        hand_data[hand]->glyph_path = GESTURE_TO_VERTICES.at(hand->gesture);
        sf::Texture* tex;
        if (hand->gesture == Gesture::Earth)
            tex = earth_tex;
        if (hand->gesture == Gesture::Air)
            tex = air_tex;
        if (hand->gesture == Gesture::Water)
            tex = water_tex;
        if (hand->gesture == Gesture::Fire)
            tex = fire_tex;
        if (hand->gesture == Gesture::Life)
            tex = life_tex;
        if (hand->gesture == Gesture::Death)
            tex = death_tex;
        glyph = sf::Sprite(*tex);
        glyph.setColor(HalfTransparent);
        glyph.setOrigin(127.5, 127.5);
        glyph.move(WIDTH/2, HEIGHT/2);
    }
    catch (const std::out_of_range& oor)
    {
        //TODO: Horz, Vert, Rotate, Cast
    }
}

void GlyphDrawer::notify_cancel(Hand* hand)
{

}

void GlyphDrawer::notify_winddown(Hand* hand)
{
    glyph = sf::Sprite();
    hand_data[hand]->glyph_path.clear();
}


void GlyphDrawer::draw(sf::RenderTarget* target)
{
    left_hand->draw(target);
    right_hand->draw(target);

    sf::Transform transform;
    transform.translate(WIDTH/2, HEIGHT/2);
    transform.scale(GLYPH_SCALE, GLYPH_SCALE);
    sf::RenderStates states;
    states.transform = transform;
    for (auto hand: {left_hand, right_hand})
    {
        target->draw(glyph);
    }
    
}

void GlyphDrawer::update(double dt)
{
    // if hand is idle have it wander
    for (auto hand: {left_hand, right_hand})
    {
        if (hand->hand_state == HandState::None && hand_data[hand]->time == 0)
        {
            double rx = double(random_int(-HAND_DRIFT*1000,HAND_DRIFT*1000))/1000.0;
            double yx = double(random_int(-HAND_DRIFT*1000,HAND_DRIFT*1000))/1000.0;
            Vector wander_pos = Vector(rx, yx);
            Vector current_pos = hand_data[hand]->current;
            double time_mod = double(random_int(800,1200)/1000.0);
            double time = (current_pos - wander_pos).magnitude()/20*time_mod;
            if (time <= 0) time = 0.1;

            hand_data[hand]->start = current_pos;
            hand_data[hand]->end = wander_pos;
            hand_data[hand]->time = time;
            hand_data[hand]->time_elapsed = 0;

            // std::cout << "wander " << current_pos << " to " << wander_pos << " in " << time << std::endl;
        }
    }

    // set new hand positions
    for (auto hand: {left_hand, right_hand})
    {
        Vector pos = position + hand_data[hand]->base_pos;
        if (hand_data[hand]->time != 0)
        {
            double r = hand_data[hand]->time_elapsed / hand_data[hand]->time;
            Vector p = hand_data[hand]->start*(1-r) + hand_data[hand]->end*r;
            pos += p;
            hand_data[hand]->current = p;
            hand_data[hand]->time_elapsed += dt;
            if (hand_data[hand]->time_elapsed >= hand_data[hand]->time)
                hand_data[hand]->time = 0;
        }
        hand->position = pos;
        // std::cout << "pos " <<hand->position << std::endl;
    }

    // update hands
    left_hand->update(dt);
    right_hand->update(dt);
}

void GlyphDrawer::input(bool left, int finger, bool in)
{
    if (left)
        left_hand->input(finger, in);
    else
        right_hand->input(finger, in);
}