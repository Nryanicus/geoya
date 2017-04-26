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
        if (hand->gesture == other_hand[hand]->gesture)
        {
            std::cout << "other gesture same, modifying" << std::endl;
            hand_data[hand]->state = HandDataState::Modifying;
            HandData* other = hand_data[other_hand[hand]];
            // TODO: scale
            // other->glyph_scale = Interp1D(1.0, 2.0, GLYPH_DRAW_TIME);
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
        hand_data[hand]->state = HandDataState::Modifying;
        std::cout << "modifying gesture" << std::endl;
        //TODO: Horz, Vert, Rotate, Cast
    }
}

void GlyphDrawer::notify_cancel(Hand* hand)
{

}

void GlyphDrawer::notify_winddown(Hand* hand)
{
    hand_data[hand]->move_path.clear();
    hand_data[hand]->glyph_outline->clear();

    // cancel on-going mod, we're modding other hand
    if (hand_data[hand]->state == HandDataState::Modifying && hand_data[other_hand[hand]]->state == HandDataState::Modifying)
    {

    }
    // cancel on-going mod, other hand is modding us
    if (hand_data[other_hand[hand]]->state == HandDataState::Modifying && hand_data[hand]->state == HandDataState::Modifying)
    {

    }

    hand_data[hand]->state = HandDataState::Moving;
    double time = std::min( 5*GESTURE_FRAME_TIME, 0.41);
    if (time <= 0) time = 0.1;
    hand_data[hand]->hand_translate = Interp2D(hand_data[hand]->current, Vector(0, 0), time);
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
        if (hand_data[hand]->state == HandDataState::Drawing)
        {
            // move
            double r = hand->hand_time/GLYPH_DRAW_TIME*(hand_data[hand]->move_path.size()-1);
            if (r > hand_data[hand]->move_path.size()-1)
                r = hand_data[hand]->move_path.size()-1;
            Vector p1 = hand_data[hand]->move_path[floor(r)] * GLYPH_MOVEMENT_SCALE;
            Vector p2 = hand_data[hand]->move_path[ceil(r)] * GLYPH_MOVEMENT_SCALE;
            double r_d = r - floor(r);
            Vector p = p1*(1-r_d) + p2*r_d;
            hand_data[hand]->current = p;
            pos += p;

            // draw
            hand_data[hand]->current_glyph->incremental_draw(hand->hand_time);

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