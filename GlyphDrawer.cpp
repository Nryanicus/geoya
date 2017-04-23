#include "GlyphDrawer.hpp"

GlyphDrawer::GlyphDrawer(sf::Texture* top_hand_tex)
: position(BASE_POS)
{
    left_hand = new Hand(top_hand_tex, this, "TopBack", false);
    right_hand = new Hand(top_hand_tex, this, "TopBack", true);

    left_hand_data.base_pos = LEFTHAND_POS;
    right_hand_data.base_pos = RIGHTHAND_POS;

    hand_data[left_hand] = &left_hand_data;
    hand_data[right_hand] = &right_hand_data;

    other_hand[left_hand] = left_hand;
    other_hand[right_hand] = right_hand;

    for (auto hand: {left_hand, right_hand})
    {
        hand_data[hand]->glyph_outline.create(2.2*GLYPH_SCALE, 2.2*GLYPH_SCALE);
        hand_data[hand]->glyph_outline.setSmooth(true);
        hand_data[hand]->prev_valid = false;
    }

    glyph.create(2.2*GLYPH_SCALE, 2.2*GLYPH_SCALE);
    glyph.setSmooth(true);
    glyph.clear(Transparent);
    glyph.display();
}

GlyphDrawer::~GlyphDrawer()
{
    delete left_hand;
    delete right_hand;
}

void GlyphDrawer::notify_windup(Hand* hand)
{
        
    hand_data[hand]->start = hand_data[hand]->current;
    if (is_drawable(hand->gesture))
        hand_data[hand]->end = GESTURE_TO_MOVE.at(hand->gesture)[0] * GLYPH_MOVEMENT_SCALE;
    else
        hand_data[hand]->end = Vector(0,0);
    // have hand travel at ~30pixels/sec(?) or in 0.41 (just short of the time to complete a WindUp anim), whichever is smaller
    hand_data[hand]->time = std::min( 5*GESTURE_FRAME_TIME, 0.41);
    if (hand_data[hand]->time <= 0) hand_data[hand]->time = 0.1;
    hand_data[hand]->time_elapsed = 0;
}

void GlyphDrawer::notify_hold(Hand* hand)
{
    hand_data[hand]->time = 0;
    if (is_drawable(hand->gesture))
    {
        hand_data[hand]->glyph_path = GESTURE_TO_GLYPH.at(hand->gesture);
        hand_data[hand]->move_path = GESTURE_TO_MOVE.at(hand->gesture);
        
        sf::Color col = GESTURE_TO_COLOUR.at(hand->gesture);
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
            hand_data[hand]->glyph_outline.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS, col));
            prev = hand_data[hand]->glyph_path.at(i);
        }
        // make transparent
        sf::RectangleShape rect(sf::Vector2f(hand_data[hand]->glyph_outline.getSize()));
        rect.setFillColor(sf::Color(255,255,255,64));
        hand_data[hand]->glyph_outline.draw(rect, sf::RenderStates(sf::BlendMultiply));

        hand_data[hand]->glyph_outline.display();
    }
    else
    {
        //TODO: Horz, Vert, Rotate, Cast
    }
}

void GlyphDrawer::notify_cancel(Hand* hand)
{

}

void GlyphDrawer::notify_winddown(Hand* hand)
{
    hand_data[hand]->glyph_path.clear();
    hand_data[hand]->glyph_outline.clear(Transparent);
    hand_data[hand]->prev_valid = false;

    hand_data[hand]->start = hand_data[hand]->current;
    hand_data[hand]->end = Vector(0, 0);
    hand_data[hand]->time = std::min( 5*GESTURE_FRAME_TIME, 0.41);
    if (hand_data[hand]->time <= 0) hand_data[hand]->time = 0.1;
    hand_data[hand]->time_elapsed = 0;
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
    draw_sprite.setTexture(glyph.getTexture());
    draw_sprite.setOrigin(draw_sprite.getTextureRect().width/2, draw_sprite.getTextureRect().height/2);
    draw_sprite.setPosition(WIDTH/2, HEIGHT/2);
    target->draw(draw_sprite);
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
        }
    }

    // set new hand positions
    for (auto hand: {left_hand, right_hand})
    {
        Vector pos = position + hand_data[hand]->base_pos;
        // move hand along Gesture path, colour in Glyph as hand travels
        if (hand->hand_state == HandState::Hold && is_drawable(hand->gesture) && hand->hand_time <= GLYPH_DRAW_TIME)
        {
            // move
            double r = hand->hand_time/GLYPH_DRAW_TIME*(hand_data[hand]->move_path.size()-1);
            Vector p1 = hand_data[hand]->move_path[floor(r)] * GLYPH_MOVEMENT_SCALE;
            Vector p2 = hand_data[hand]->move_path[ceil(r)] * GLYPH_MOVEMENT_SCALE;
            double r_d = r - floor(r);
            Vector p = p1*(1-r_d) + p2*r_d;
            hand_data[hand]->current = p;
            pos += p;

            // draw
            sf::Color col = GESTURE_TO_COLOUR.at(hand->gesture);
            r = hand->hand_time/GLYPH_DRAW_TIME*(hand_data[hand]->glyph_path.size()-1);
            if (!hand_data[hand]->prev_valid)
            {
                Vector prev = hand_data[hand]->glyph_path[0];
                // this is almost certainly only going to be the first line, or nothing at all
                for (int i=1; i<floor(r); i++)
                {
                    p1 = ((prev+Vector(1.1,1.1))*GLYPH_SCALE);
                    p2 = ((hand_data[hand]->glyph_path.at(i)+Vector(1.1,1.1))*GLYPH_SCALE);
                    glyph.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS/2, col));
                    prev = hand_data[hand]->glyph_path.at(i);
                }
                hand_data[hand]->prev_valid = true;
                hand_data[hand]->prev_glyph_draw_point = prev;
            }
            // don't draw if we're in the middle of the line break for Air or Death
            p1 = hand_data[hand]->glyph_path[floor(r)];
            p2 = hand_data[hand]->glyph_path[ceil(r)];
            r_d = r - floor(r);
            p = p1*(1-r_d) + p2*r_d;
            if (TWO_LINE_GESTURE.find(hand->gesture) != TWO_LINE_GESTURE.end())
            {
                if (r >= hand_data[hand]->glyph_path.size()/2-1 && r <= hand_data[hand]->glyph_path.size()/2)
                    goto move_hand;
                else if (floor(r) == hand_data[hand]->glyph_path.size()/2 && hand_data[hand]->prev_glyph_draw_point != p1)
                {
                    hand_data[hand]->prev_glyph_draw_point = p1;
                    goto move_hand;
                }
            }
            p1 = (hand_data[hand]->prev_glyph_draw_point+Vector(1.1,1.1))*GLYPH_SCALE;
            p2 = (p+Vector(1.1,1.1))*GLYPH_SCALE;
            glyph.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS/2, col));
            hand_data[hand]->prev_glyph_draw_point = p;
            glyph.display();
        }
        else if (hand->hand_state == HandState::Hold)
            pos += hand_data[hand]->current;

        // wander/interpolate
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
        move_hand:
        hand->position = pos;
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