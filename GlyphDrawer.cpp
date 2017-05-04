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

    other_hand[left_hand] = right_hand;
    other_hand[right_hand] = left_hand;
}

GlyphDrawer::~GlyphDrawer()
{
    delete left_hand;
    delete right_hand;
    for (int i=0; i<glyphs.size(); i++)
        delete glyphs[i];
}

void GlyphDrawer::notify_windup(Hand* hand)
{
    hand_data[hand]->state = HandDataState::Moving;
    Vector end;
    if (is_drawable(hand->gesture))
        end = GESTURE_TO_MOVE.at(hand->gesture)[0] * GLYPH_MOVEMENT_SCALE;
    else
        end = Vector(0, 0);
    if (hand == left_hand)
        end.x *= -1;
    double time = std::min( 5*GESTURE_FRAME_TIME, 0.41);
    if (time <= 0) time = 0.1;
    hand_data[hand]->hand_translate = Interp2D(hand_data[hand]->current, end, time);
}

void GlyphDrawer::notify_hold(Hand* hand)
{
    HandData* other = hand_data[other_hand[hand]];
    if (is_drawable(hand->gesture))
    {
        hand_data[hand]->move_path = GESTURE_TO_MOVE.at(hand->gesture);
        // modify, scale
        if (hand->gesture == other_hand[hand]->gesture 
            && other->state == HandDataState::Drawing)
        {
            hand_data[hand]->current_glyph = other->current_glyph;
            hand_data[hand]->state = HandDataState::Modifying;
            other->current_glyph->set_scale_interp();
        }
        // if other hand is done drawing, just hold it
        else if (hand->gesture == other_hand[hand]->gesture 
            && (other->current_glyph && other->current_glyph->complete))
        {
            hand_data[hand]->current_glyph = other->current_glyph;
            hand_data[hand]->state = HandDataState::Holding;
        }
        // new glyph
        else
        {
            hand_data[hand]->state = HandDataState::Drawing;
            hand_data[hand]->current_glyph = new Glyph(hand->gesture, hand==left_hand);
            glyphs.push_back(hand_data[hand]->current_glyph);
        }
    }
    else
    {
        // modify, other
        hand_data[hand]->state = HandDataState::Holding;
        if (! (other->current_glyph && is_drawable(other_hand[hand]->gesture)))
            return;
        hand_data[hand]->current_glyph = other->current_glyph;
        if (hand->gesture == Gesture::Rotate)
        {
            double end = 45;
            if (hand == left_hand)
                end *= -1;
            hand_data[hand]->current_glyph->set_rotation_interp(end);
        }
        //TODO: Horz, Vert
    }
}

bool GlyphDrawer::notify_cast(Hand* hand)
{
    if (other_hand[hand]->hand_state == HandState::CastReady)
    {
        other_hand[hand]->gesture = Gesture::Cast;
        other_hand[hand]->hand_state = HandState::Casting;
        other_hand[hand]->dirty = true;

        hand_data[hand]->state = HandDataState::Casting;
        hand_data[other_hand[hand]]->state = HandDataState::Casting;
        return true;
    }
    return false;
}

bool GlyphDrawer::notify_cast_complete(Hand* hand)
{
    glyphs.clear();
}

void GlyphDrawer::notify_cancel(Hand* hand)
{

}

void GlyphDrawer::notify_winddown(Hand* hand)
{
    // only possible mod here is Scale as others happen instantaneously

    // cancel on-going mod, we're modding other hand
    HandData* other = hand_data[other_hand[hand]];
    if (hand_data[hand]->state == HandDataState::Modifying 
        && (other->state == HandDataState::Drawing || other->state == HandDataState::Holding))
    {
        other->current_glyph->reverse_scale();
    }
    // cancel on-going mod, other hand is modding us
    else if (other->state == HandDataState::Modifying 
        && (hand_data[hand]->state == HandDataState::Drawing || hand_data[hand]->state == HandDataState::Holding))
    {
        // TODO animated the difference
        other->current_glyph->reverse_scale();
        other->current_glyph->reset(hand==right_hand);
        other->state = HandDataState::Drawing;
    }
    // no mods going on
    else
    {
        // if we didn't finish drawing the current glyph kill it, TODO ANIM
        if (is_drawable(hand->gesture) && hand_data[hand]->current_glyph && ! hand_data[hand]->current_glyph->complete)
            glyphs.erase(std::remove(glyphs.begin(), glyphs.end(), hand_data[hand]->current_glyph), glyphs.end());
    }
    // move hand to centre
    hand_data[hand]->move_path.clear();
    hand_data[hand]->state = HandDataState::Moving;
    double time = std::min( 5*GESTURE_FRAME_TIME, 0.41);
    if (time <= 0) time = 0.1;
    hand_data[hand]->hand_translate = Interp2D(hand_data[hand]->current, Vector(0, 0), time);
    hand_data[hand]->current_glyph = NULL;
}

void GlyphDrawer::draw(sf::RenderTarget* target)
{
    left_hand->draw(target);
    right_hand->draw(target);

    for (auto glyph: glyphs)
    {
        if (! glyph->complete)
            glyph->draw_outline(target);
        glyph->draw(target);
    }
}

void GlyphDrawer::update(double dt)
{
    // if hand is idle have it wander
    for (auto hand: {left_hand, right_hand})
    {
        if (hand_data[hand]->state == HandDataState::None)
        {
            double rx = double(random_int(-HAND_DRIFT*1000,HAND_DRIFT*1000))/1000.0;
            double yx = double(random_int(-HAND_DRIFT*1000,HAND_DRIFT*1000))/1000.0;
            Vector wander_pos = Vector(rx, yx);
            Vector current_pos = hand_data[hand]->current;
            double time_mod = double(random_int(800,1200)/1000.0);
            double time = (current_pos - wander_pos).magnitude()/20*time_mod;
            if (time <= 0) time = 0.1;

            hand_data[hand]->state = HandDataState::Wandering;
            hand_data[hand]->hand_translate = Interp2D(hand_data[hand]->current, wander_pos, time);
        }
    }

    // set new hand positions
    for (auto hand: {left_hand, right_hand})
    {
        Vector pos = position + hand_data[hand]->base_pos;
        // move hand along Gesture path, colour in Glyph as hand travels
        if (hand_data[hand]->state == HandDataState::Drawing || hand_data[hand]->state == HandDataState::Modifying)
        {
            // move
            double r = hand->hand_time/GLYPH_DRAW_TIME*(hand_data[hand]->move_path.size()-1);
            if (r > hand_data[hand]->move_path.size()-1)
                r = hand_data[hand]->move_path.size()-1;
            Vector p1 = hand_data[hand]->move_path[floor(r)] * GLYPH_MOVEMENT_SCALE;
            Vector p2 = hand_data[hand]->move_path[ceil(r)] * GLYPH_MOVEMENT_SCALE;
            double r_d = r - floor(r);
            Vector p = p1*(1-r_d) + p2*r_d;
            if (hand == left_hand)
                p.x *= -1;
            hand_data[hand]->current = p;
            pos += p;

            // draw
            if (hand_data[hand]->state == HandDataState::Drawing)
                hand_data[hand]->current_glyph->incremental_draw(hand->hand_time);

            // if we're done, change state
            if (hand->hand_time >= GLYPH_DRAW_TIME)
            {
                hand_data[hand]->state = HandDataState::Holding;
                hand_data[hand]->current_glyph->complete = true;
            }
        }
        // hold hand at endpoint of the glyph
        else if (hand_data[hand]->state == HandDataState::Holding)
            pos += hand_data[hand]->current;

        // wander/interpolate
        if (hand_data[hand]->state == HandDataState::Wandering || hand_data[hand]->state == HandDataState::Moving)
        {
            Vector p = hand_data[hand]->hand_translate.get_point();
            pos += p;
            hand_data[hand]->current = p;
            if (hand_data[hand]->hand_translate.update(dt))
                hand_data[hand]->state = HandDataState::None;
        }
        hand->position = pos;
    }
    for (auto glyph: glyphs)
        glyph->update(dt);
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