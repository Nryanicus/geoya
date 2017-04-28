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
        end = Vector(0,0);
    double time = std::min( 5*GESTURE_FRAME_TIME, 0.41);
    if (time <= 0) time = 0.1;
    hand_data[hand]->hand_translate = Interp2D(hand_data[hand]->current, end, time);
}

void GlyphDrawer::notify_hold(Hand* hand)
{
    if (is_drawable(hand->gesture))
    {
        hand_data[hand]->move_path = GESTURE_TO_MOVE.at(hand->gesture);
        if (hand->gesture == other_hand[hand]->gesture 
            && (hand_data[other_hand[hand]]->state == HandDataState::Drawing || hand_data[other_hand[hand]]->state == HandDataState::Holding))
        {
            hand_data[hand]->state = HandDataState::Modifying;
            HandData* other = hand_data[other_hand[hand]];
            other->glyph_outline->set_scale_interp();
            other->current_glyph->set_scale_interp();
            return;
        }
        hand_data[hand]->state = HandDataState::Drawing;
        if (hand_data[hand]->glyph_outline)
            delete hand_data[hand]->glyph_outline;
        hand_data[hand]->glyph_outline = new GlyphOutline(hand->gesture);
        hand_data[hand]->current_glyph = new Glyph(hand->gesture);
        glyphs.push_back(hand_data[hand]->current_glyph);
    }
    else
    {
        hand_data[hand]->state = HandDataState::Holding;
        //TODO: Horz, Vert, Rotate, Cast

    }
}

bool GlyphDrawer::notify_cast(Hand* hand)
{
    if (hand == left_hand)
        std::cout << "L:";
    else
        std::cout << "R:";
    std::cout << " notify cast" << std::endl;
    if (other_hand[hand]->hand_state == HandState::CastReady)
    {
        std::cout << "other is ready, casting" << std::endl;
        other_hand[hand]->gesture = Gesture::Cast;
        other_hand[hand]->hand_state = HandState::Casting;
        return true;
    }
    return false;
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
        other->glyph_outline->reverse_scale();
        other->current_glyph->reverse_scale();
    }
    // cancel on-going mod, other hand is modding us
    else if (other->state == HandDataState::Modifying 
        && (hand_data[hand]->state == HandDataState::Drawing || hand_data[hand]->state == HandDataState::Holding))
    {
        other->swap(hand_data[hand], hand->gesture);
        delete hand_data[hand]->current_glyph;
        hand_data[hand]->glyph_outline->clear();
        // remove the now-invalid glyph
        glyphs.pop_back();
        // add new one
        glyphs.push_back(other->current_glyph);
    }
    else
    {
        hand_data[hand]->move_path.clear();
        if (hand_data[hand]->glyph_outline)
            hand_data[hand]->glyph_outline->clear();
    }

    hand_data[hand]->state = HandDataState::Moving;
    double time = std::min( 5*GESTURE_FRAME_TIME, 0.41);
    if (time <= 0) time = 0.1;
    // hand_data[hand]->hand_translate = Interp2D(hand_data[hand]->current, Vector(0, 0), time);
}

void GlyphDrawer::draw(sf::RenderTarget* target)
{
    left_hand->draw(target);
    right_hand->draw(target);

    for (auto hand: {left_hand, right_hand})
        if (hand_data[hand]->glyph_outline)
            hand_data[hand]->glyph_outline->draw(target);

    for (auto glyph: glyphs)
        glyph->draw(target);
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
            // std::cout << hand_data[hand]->move_path.size() << std::endl;
            Vector p1 = hand_data[hand]->move_path[floor(r)] * GLYPH_MOVEMENT_SCALE;
            Vector p2 = hand_data[hand]->move_path[ceil(r)] * GLYPH_MOVEMENT_SCALE;
            double r_d = r - floor(r);
            Vector p = p1*(1-r_d) + p2*r_d;
            hand_data[hand]->current = p;
            pos += p;

            // draw
            if (hand_data[hand]->state == HandDataState::Drawing)
                hand_data[hand]->current_glyph->incremental_draw(hand->hand_time);

            // if we're done, change state
            if (hand->hand_time >= GLYPH_DRAW_TIME)
                hand_data[hand]->state = HandDataState::Holding;
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
        if (hand_data[hand]->glyph_outline)
            hand_data[hand]->glyph_outline->update(dt);
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