#include "Hand.hpp"

Hand::Hand(sf::Texture* tex, GlyphDrawer* parent, std::string angle, bool right)
: right(right), angle(angle), parent(parent)
{
    working.setTexture(*tex);
    canvas.create(1000, 1000);
    canvas.setSmooth(true);
    dirty = true;
    gesture = Gesture::None;
    hand_state = HandState::None;
    hand_frame = 0;
    hand_time = 0;
    for (int i=0; i< NUM_FINGERS; i++)
    {
        finger_times.push_back(0);
        finger_frames.push_back(4);
        finger_states.push_back(FingerState::Opening);
    }
}

void Hand::draw(sf::RenderTarget* target)
{
    if (dirty)
    {
        canvas.clear(Transparent);

        if (hand_state == HandState::None || hand_state == HandState::CastReady)
        {
            for (int i=0; i<NUM_FINGERS; i++)
            {
                std::string sprite = std::to_string(i)+"Close"+std::to_string(finger_frames[i]+1);
                working.setTextureRect(spritesheet_rect(angle, sprite));
                working.setPosition(spritesheet_offset(angle, sprite));
                canvas.draw(working);
            }
            working.setTextureRect(spritesheet_rect(angle, "Palm"));
            working.setPosition(spritesheet_offset(angle, "Palm"));
            canvas.draw(working);
        }
        else 
        {
            std::string sprite = gesture_to_sprite(gesture) + std::to_string(hand_frame+1);
            working.setTextureRect(spritesheet_rect(angle, sprite));
            working.setPosition(spritesheet_offset(angle, sprite));
            canvas.draw(working);
        }
        // update canvas
        canvas.display();
        draw_sprite.setTexture(canvas.getTexture());
        dirty = false;
    }
    draw_sprite.setOrigin(draw_sprite.getTextureRect().width/2, draw_sprite.getTextureRect().height/2);
    draw_sprite.setPosition(position.to_sfml());
    if (right)
        draw_sprite.setScale(-1.0, 1.0);
    target->draw(draw_sprite);
}

void Hand::update(double dt)
{
    // std::cout << hs2s(hand_state) << " " << g2s(gesture) << std::endl;
    // std::cout << hand_time << " " << hand_frame << std::endl;
    // for (int i=0; i<NUM_FINGERS; i++)
    //     std::cout << i << ": " << fs2s(finger_states[i]) << " " << finger_times[i] << " " << finger_frames[i] << " | ";
    // std::cout << std::endl;
    // std::cout << "==================================" << std::endl;
    if (hand_state == HandState::CastReady) return;
    if (hand_state == HandState::None)
    {
        for (int i=0; i<NUM_FINGERS; i++)
        {
            if (finger_states[i] == FingerState::Opening)
            {
                finger_times[i] += dt;
                if (finger_times[i] >= FINGER_FRAME_TIME)
                {
                    dirty = true;
                    finger_times[i] -= FINGER_FRAME_TIME;
                    finger_frames[i]--;
                    if (finger_frames[i] <= 0)
                    {
                        finger_states[i] = FingerState::Open;
                        finger_frames[i] = 0;
                        gesture = check_gesture();
                        if (gesture != Gesture::None)
                        {
                            hand_state = HandState::WindUp;
                            parent->notify_windup(this);
                        }
                    }
                }
            }
            if (finger_states[i] == FingerState::Closing)
            {
                finger_times[i] += dt;
                if (finger_times[i] >= FINGER_FRAME_TIME)
                {
                    dirty = true;
                    finger_times[i] -= FINGER_FRAME_TIME;
                    finger_frames[i]++;
                    if (finger_frames[i] >= MAX_FRAMES)
                    {
                        finger_frames[i] = MAX_FRAMES;
                        finger_states[i] = FingerState::Closed;
                        gesture = check_gesture();
                        // Cast can only happen on closing fingers
                        if (gesture == Gesture::Cast)
                        {
                            hand_state = HandState::CastReady;
                            // if the other hand is ready to cast go ahead
                            if (parent->notify_cast(this))
                                hand_state = HandState::Casting;
                            else
                                gesture = Gesture::None;
                            return;
                        }
                        else
                            hand_state = HandState::None;
                        if (gesture != Gesture::None)
                        {
                            hand_state = HandState::WindUp;
                            parent->notify_windup(this);
                        }
                    }
                }
            }
        }
    }
    else
    {
        hand_time += dt;
        if (hand_state == HandState::Hold)
            return;
        if (hand_time >= GESTURE_FRAME_TIME)
        {
            hand_time -= GESTURE_FRAME_TIME;
            dirty = true;
            if (hand_state == HandState::WindUp || hand_state == HandState::Casting)
            {
                hand_frame++;
                if (hand_frame >= MAX_FRAMES)
                {
                    hand_frame = MAX_FRAMES;
                    hand_state = HandState::Hold;
                    hand_time = 0;
                    parent->notify_hold(this);
                    // TODO: send signal to glyph drawer
                }
            }
            // either Cancelled or WindDown, same either way
            else
            {
                hand_frame--;
                if (hand_frame <= 0)
                {
                    gesture = Gesture::None;
                    hand_state = HandState::None;
                    hand_frame = 0;
                }
            }
        }
    }
}

Gesture Hand::check_gesture()
{
    Gesture out = Gesture::None;
    if (finger_states[0] == FingerState::Open &&
        finger_states[1] == FingerState::Open &&
        finger_states[2] == FingerState::Closed &&
        finger_states[3] == FingerState::Open &&
        finger_states[4] == FingerState::Open)
    {
        out = Gesture::Earth;
    }
    else if (finger_states[0] == FingerState::Open &&
        finger_states[1] == FingerState::Closed &&
        finger_states[2] == FingerState::Open &&
        finger_states[3] == FingerState::Closed &&
        finger_states[4] == FingerState::Open)
    {
        out = Gesture::Fire;
    }
    else if (finger_states[0] == FingerState::Closed &&
        finger_states[1] == FingerState::Open &&
        finger_states[2] == FingerState::Closed &&
        finger_states[3] == FingerState::Closed &&
        finger_states[4] == FingerState::Open)
    {
        out = Gesture::Air;
    }
    else if (finger_states[0] == FingerState::Open &&
        finger_states[1] == FingerState::Open &&
        finger_states[2] == FingerState::Closed &&
        finger_states[3] == FingerState::Closed &&
        finger_states[4] == FingerState::Closed)
    {
        out = Gesture::Water;
    }
    else if (finger_states[0] == FingerState::Closed &&
        finger_states[1] == FingerState::Closed &&
        finger_states[2] == FingerState::Open &&
        finger_states[3] == FingerState::Closed &&
        finger_states[4] == FingerState::Closed)
    {
        out = Gesture::Death;
    }
    else if (finger_states[0] == FingerState::Open &&
        finger_states[1] == FingerState::Closed &&
        finger_states[2] == FingerState::Closed &&
        finger_states[3] == FingerState::Closed &&
        finger_states[4] == FingerState::Closed)
    {
        out = Gesture::Life;
    }
    else if (finger_states[0] == FingerState::Closed &&
        finger_states[1] == FingerState::Open &&
        finger_states[2] == FingerState::Open &&
        finger_states[3] == FingerState::Closed &&
        finger_states[4] == FingerState::Closed)
    {
        out = Gesture::Horz;
    }
    else if (finger_states[0] == FingerState::Closed &&
        finger_states[1] == FingerState::Closed &&
        finger_states[2] == FingerState::Open &&
        finger_states[3] == FingerState::Open &&
        finger_states[4] == FingerState::Closed)
    {
        out = Gesture::Vert;
    }
    else if (finger_states[0] == FingerState::Closed &&
        finger_states[1] == FingerState::Open &&
        finger_states[2] == FingerState::Open &&
        finger_states[3] == FingerState::Open &&
        finger_states[4] == FingerState::Open)
    {
        out = Gesture::Rotate;
    }
    // TODO: check with controller to see if other hand is clenched too
    else if (finger_states[0] == FingerState::Closed &&
        finger_states[1] == FingerState::Closed &&
        finger_states[2] == FingerState::Closed &&
        finger_states[3] == FingerState::Closed &&
        finger_states[4] == FingerState::Closed)
    {
        out = Gesture::Cast;
    }
    return out;
}

void Hand::input(int finger, bool in)
{
    if (in)
    {
        // if (finger_states[finger] == FingerState::Closed)
        //     std::cout << "Closed finger told to close?!" << std::endl;
        // else if (finger_states[finger] == FingerState::Closing)
        //     std::cout << "Closing finger told to close?!" << std::endl;
        if (finger_states[finger] == FingerState::Open)
        {
            finger_times[finger] = 0;
            finger_states[finger] = FingerState::Closing;
        }
        else if (finger_states[finger] == FingerState::Opening)
        {
            finger_times[finger] = 0;
            finger_states[finger] = FingerState::Closing;
        }
    }
    else
    {
        if (finger_states[finger] == FingerState::Closed)
        {
            finger_times[finger] = 0;
            finger_states[finger] = FingerState::Opening;
        }
        else if (finger_states[finger] == FingerState::Closing)
        {
            finger_times[finger] = 0;
            finger_states[finger] = FingerState::Opening;
        }
        // else if (finger_states[finger] == FingerState::Open)
        //     std::cout << "Open finger told to open?!" << std::endl;
        // else if (finger_states[finger] == FingerState::Opening)
        //     std::cout << "Opening finger told to open?!" << std::endl;
    }
    // can't interrupt gesture when there's none or if we're winding down
    if (gesture != Gesture::None && hand_state != HandState::WindDown)
    {
        if (gesture != check_gesture())
        {
            if (hand_state == HandState::WindUp)
            {
                hand_state = HandState::Cancel;
                parent->notify_cancel(this);
                hand_time = 0;
            }
            if (hand_state == HandState::Hold)
            {
                hand_state = HandState::WindDown;
                hand_time = 0;
                parent->notify_winddown(this);
            }
        }
    }
    // check if we're no longer cast ready
    else if (hand_state == HandState::CastReady && check_gesture() != Gesture::Cast)
    {
        std::cout << "no long cast read" << std::endl;
        hand_state = HandState::None;
    }
}
