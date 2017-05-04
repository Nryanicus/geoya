#ifndef glyph_hpp
#define glyph_hpp

#include <SFML/Graphics.hpp>
#include "RoundedLine.hpp"
#include "constants.hpp"
#include "utilities.hpp"

class Glyph
{
public:
    sf::RenderTexture render_texture;
    sf::RenderTexture outline_texture;

    Gesture gesture;

    // modifying
    bool rotation_active;
    Interp1D rotation_interp;
    bool translation_active;
    Interp2D translation_interp;
    bool scale_active;
    Interp1D scale_interp;

    Vector translation;
    double rotation;
    double scale;
    sf::Color col;
    std::vector<Vector> glyph_path;
    bool prev_valid;
    Vector prev_glyph_draw_point;

    bool two_line;
    bool complete;

    bool mirror;

    // full glyphs are drawn incrementally
    Glyph(Gesture g, bool m)
    : gesture(g), prev_valid(false), scale(1), rotation(0), translation_active(false), scale_active(false), rotation_active(false), complete(false), mirror(m)
    {
        render_texture.create(2.2*GLYPH_SCALE, 2.2*GLYPH_SCALE);
        render_texture.setSmooth(true);
        render_texture.clear(Transparent);
        render_texture.display();
        outline_texture.create(2.2*GLYPH_SCALE, 2.2*GLYPH_SCALE);
        outline_texture.setSmooth(true);
        outline_texture.clear(Transparent);
        outline_texture.display();
        // drawing
        col = GESTURE_TO_COLOUR.at(gesture);
        glyph_path = GESTURE_TO_GLYPH.at(gesture);

        two_line = TWO_LINE_GESTURE.find(gesture) != TWO_LINE_GESTURE.end();

        // create outline
        Vector prev = glyph_path.at(0);
        if (mirror)
            prev.x *= -1;
        for (int i=1; i<glyph_path.size(); i++)
        {
            Vector p = glyph_path.at(i);
            if (mirror)
                p.x *= -1;
            // skip halfway through for Air and Death (the two-line glyphs)
            if (two_line && i == glyph_path.size()/2)
            {
                prev = p;
                continue;
            }
            Vector p1 = ((prev+Vector(1.1,1.1))*GLYPH_SCALE);
            Vector p2 = ((p+Vector(1.1,1.1))*GLYPH_SCALE);
            outline_texture.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS, col));
            prev = p;
        }
        // make transparent
        sf::RectangleShape rect(sf::Vector2f(outline_texture.getSize()));
        rect.setFillColor(sf::Color(255,255,255,64));
        outline_texture.draw(rect, sf::RenderStates(sf::BlendMultiply));

        outline_texture.display();
    }

    void reset(bool m)
    {
        mirror = m;
        prev_valid = false;
        render_texture.clear(Transparent);
    }

    void incremental_draw(double time_elapsed)
    {
        double r = time_elapsed/GLYPH_DRAW_TIME*(glyph_path.size()-1);
        if (r > glyph_path.size()-1)
            r = glyph_path.size()-1;

        double r_d = r - floor(r);
        if (!prev_valid)
        {
            Vector prev = glyph_path[0];
            if (mirror)
                prev.x *= -1;
            // this is almost certainly only going to be the first line, or nothing at all
            for (int i=1; i<ceil(r); i++)
            {
                Vector p = glyph_path.at(i);
                if (mirror)
                    p.x *= -1;
                Vector p1 = ((prev+Vector(1.1,1.1))*GLYPH_SCALE);
                Vector p2 = ((p+Vector(1.1,1.1))*GLYPH_SCALE);
                render_texture.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS/2, col));
                prev = p;
            }
            prev_valid = true;
            prev_glyph_draw_point = prev;
        }
        // don't draw if we're in the middle of the line break for Air or Death
        Vector p1 = glyph_path[floor(r)];
        Vector p2 = glyph_path[ceil(r)];
        Vector p = p1*(1-r_d) + p2*r_d;
        if (mirror)
            p.x *= -1;
        if (two_line)
        {
            if (r >= glyph_path.size()/2-1 && r <= glyph_path.size()/2)
                return;
            else if (floor(r) == glyph_path.size()/2 && prev_glyph_draw_point != p1)
            {
                prev_glyph_draw_point = p1;
                if (mirror)
                    prev_glyph_draw_point.x *= -1;
                return;
            }
        }
        p1 = (prev_glyph_draw_point+Vector(1.1,1.1))*GLYPH_SCALE;
        p2 = (p+Vector(1.1,1.1))*GLYPH_SCALE;
        render_texture.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS/2, col));
        prev_glyph_draw_point = p;
        render_texture.display();
    }

    void clear()
    {
        render_texture.clear(Transparent);
        render_texture.display();
    }

    void draw(sf::RenderTarget* target)
    {
        sf::Sprite draw_sprite;
        draw_sprite.setTexture(render_texture.getTexture());
        draw_sprite.setOrigin(draw_sprite.getTextureRect().width/2, draw_sprite.getTextureRect().height/2);
        draw_sprite.setPosition(WIDTH/2 + translation.x, HEIGHT/2 + translation.y);
        draw_sprite.setScale(scale, scale);
        draw_sprite.setRotation(rotation);
        target->draw(draw_sprite);
    }

    void draw_outline(sf::RenderTarget* target)
    {
        sf::Sprite draw_sprite;
        draw_sprite.setTexture(outline_texture.getTexture());
        draw_sprite.setOrigin(draw_sprite.getTextureRect().width/2, draw_sprite.getTextureRect().height/2);
        draw_sprite.setPosition(WIDTH/2 + translation.x, HEIGHT/2 + translation.y);
        draw_sprite.setScale(scale, scale);
        draw_sprite.setRotation(rotation);
        target->draw(draw_sprite);
    }

    void update(double dt)
    {
        if (scale_active)
        {
            if (scale_interp.update(dt))
                scale_active = false;
            scale = scale_interp.get_point();
        }
        if (translation_active)
        {
            if (translation_interp.update(dt))
                translation_active = false;
            translation = translation_interp.get_point();
        }
        if (rotation_active)
        {
            if (rotation_interp.update(dt))
                rotation_active = false;
            rotation = rotation_interp.get_point();
        }
    }

    void reverse_scale()
    {
        if (scale_active)
            scale_interp = Interp1D(scale_interp.get_point(), 1.0, GLYPH_DRAW_TIME-scale_interp.time_elapsed, InterpType::CubicBezier);
    }

    void set_scale_interp(double start=1.0, double end=2.0, double time=GLYPH_DRAW_TIME)
    {
        scale_active = true;
        scale_interp = Interp1D(start, end, time, InterpType::CubicBezier);
    }

    void set_rotation_interp(double end=45, double time=GLYPH_ROTATION_TIME)
    {
        // if we're already rotating do nothing
        if (rotation_active)
            return;
        rotation_active = true;
        rotation_interp = Interp1D(rotation, rotation+end, time, InterpType::CubicBezier);
    }
};

#endif