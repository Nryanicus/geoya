#include "GlyphDrawer.hpp"

GlyphDrawer::GlyphDrawer(sf::Texture* top_hand_tex)
{
    left_hand = new Hand(top_hand_tex, this, "TopBack", false);
    right_hand = new Hand(top_hand_tex, this, "TopBack", true);

    left_hand_data.base_pos = LEFTHAND_POS;
    right_hand_data.base_pos = RIGHTHAND_POS;

    hand_data[left_hand] = &left_hand_data;
    hand_data[right_hand] = &right_hand_data;

    for (auto hand: {left_hand, right_hand})
    {
        hand_data[hand]->glyph_outline.create(2.2*GLYPH_SCALE, 2.2*GLYPH_SCALE);
        hand_data[hand]->glyph_outline.setSmooth(true);
    }

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
    //DEBUG
    bool debug = false;
    if (hand->gesture == Gesture::Horz)
    {
        hand->gesture = Gesture::Death;
        debug = true;
    }
    //DEBUG
    try
    {
        hand_data[hand]->glyph_path = GESTURE_TO_VERTICES.at(hand->gesture);
        
        sf::Color col = GESTURE_TO_COLOUR.at(hand->gesture);
        col.a = 128;
        Vector prev = hand_data[hand]->glyph_path.at(0);
        for (int i=1; i<hand_data[hand]->glyph_path.size(); i++)
        {
            // skip halfway through for Air and Death (the two-line glyphs)
            if ((hand->gesture == Gesture::Air || hand->gesture == Gesture::Death) && i == hand_data[hand]->glyph_path.size()/2)
            {
                prev = hand_data[hand]->glyph_path.at(i);
                continue;
            }
            Vector p1 = ((prev+Vector(1.1,1.1))*GLYPH_SCALE);
            Vector p2 = ((hand_data[hand]->glyph_path.at(i)+Vector(1.1,1.1))*GLYPH_SCALE);
            // if (i == 1)
            //     hand_data[hand]->glyph_outline.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS, White));
            // else
            hand_data[hand]->glyph_outline.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS, col));
            prev = hand_data[hand]->glyph_path.at(i);
        }
        // complete the loop for glyphs that are closed
        if (!(hand->gesture == Gesture::Air || hand->gesture == Gesture::Death || hand->gesture == Gesture::Life))
        {
            Vector p1 = ((prev+Vector(1.1,1.1))*GLYPH_SCALE);
            Vector p2 = ((hand_data[hand]->glyph_path.at(0)+Vector(1.1,1.1))*GLYPH_SCALE);
            hand_data[hand]->glyph_outline.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS, col));
        }
        hand_data[hand]->glyph_outline.display();
    }
    catch (const std::out_of_range& oor)
    {
        //TODO: Horz, Vert, Rotate, Cast
    }
    if (debug) hand->gesture = Gesture::Horz;
}

void GlyphDrawer::notify_cancel(Hand* hand)
{

}

void GlyphDrawer::notify_winddown(Hand* hand)
{
    hand_data[hand]->glyph_path.clear();
    hand_data[hand]->glyph_outline.clear(Transparent);
}


void GlyphDrawer::draw(sf::RenderTarget* target)
{
    left_hand->draw(target);
    right_hand->draw(target);

    for (auto hand: {left_hand, right_hand})
    {
        draw_sprite.setTexture(hand_data[hand]->glyph_outline.getTexture());
        draw_sprite.setOrigin(draw_sprite.getTextureRect().width/2, draw_sprite.getTextureRect().height/2);
        draw_sprite.setPosition(WIDTH/2, HEIGHT/2);
        target->draw(draw_sprite);
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