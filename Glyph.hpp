#ifndef glyph_hpp
#define glyph_hpp

#include <SFML/Graphics.hpp>
#include "RoundedLine.hpp"
#include "constants.hpp"
#include "utilities.hpp"

class GlyphAbstract
{
public:
    sf::RenderTexture render_texture;

    // modifying
    bool translate_active;
    Interp2D translate_interp;
    bool scale_active;
    Interp1D scale_interp;

    double rotation;
    double scale;

    GlyphAbstract()
    : scale(1), rotation(0), translate_active(false), scale_active(false)
    {
        render_texture.create(2.2*GLYPH_SCALE, 2.2*GLYPH_SCALE);
        render_texture.setSmooth(true);
        render_texture.clear(Transparent);
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
        draw_sprite.setPosition(WIDTH/2, HEIGHT/2);
        draw_sprite.setScale(scale, scale);
        draw_sprite.setRotation(rotation);
        target->draw(draw_sprite);
    }
};

class Glyph : public GlyphAbstract
{
public:
    sf::Color col;
    std::vector<Vector> glyph_path;
    bool prev_valid;
    Vector prev_glyph_draw_point;

    bool two_line;

    // full glyphs are drawn incrementally
    Glyph(Gesture gesture)
    : prev_valid(false)
    {
        // drawing
        col = GESTURE_TO_COLOUR.at(gesture);
        glyph_path = GESTURE_TO_GLYPH.at(gesture);

        two_line = TWO_LINE_GESTURE.find(gesture) != TWO_LINE_GESTURE.end();
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
            // this is almost certainly only going to be the first line, or nothing at all
            for (int i=1; i<floor(r); i++)
            {
                Vector p1 = ((prev+Vector(1.1,1.1))*GLYPH_SCALE);
                Vector p2 = ((glyph_path.at(i)+Vector(1.1,1.1))*GLYPH_SCALE);
                render_texture.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS/2, col));
                prev = glyph_path.at(i);
            }
            prev_valid = true;
            prev_glyph_draw_point = prev;
        }
        // don't draw if we're in the middle of the line break for Air or Death
        Vector p1 = glyph_path[floor(r)];
        Vector p2 = glyph_path[ceil(r)];
        Vector p = p1*(1-r_d) + p2*r_d;
        if (two_line)
        {
            if (r >= glyph_path.size()/2-1 && r <= glyph_path.size()/2)
                return;
            else if (floor(r) == glyph_path.size()/2 && prev_glyph_draw_point != p1)
            {
                prev_glyph_draw_point = p1;
                return;
            }
        }
        p1 = (prev_glyph_draw_point+Vector(1.1,1.1))*GLYPH_SCALE;
        p2 = (p+Vector(1.1,1.1))*GLYPH_SCALE;
        render_texture.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS/2, col));
        prev_glyph_draw_point = p;
        render_texture.display();
    }
};

class GlyphOutline : public GlyphAbstract
{
public:
    // outline
    GlyphOutline(Gesture gesture)
    {
        sf::Color col = GESTURE_TO_COLOUR.at(gesture);
        std::vector<Vector> glyph_path = GESTURE_TO_GLYPH.at(gesture);
        Vector prev = glyph_path.at(0);
        for (int i=1; i<glyph_path.size(); i++)
        {
            // skip halfway through for Air and Death (the two-line glyphs)
            if ((gesture == Gesture::Air || gesture == Gesture::Death) && i == glyph_path.size()/2)
            {
                prev = glyph_path.at(i);
                continue;
            }
            Vector p1 = ((prev+Vector(1.1,1.1))*GLYPH_SCALE);
            Vector p2 = ((glyph_path.at(i)+Vector(1.1,1.1))*GLYPH_SCALE);
            render_texture.draw(RoundedLine(p1.to_sfml(), p2.to_sfml(), GLYPH_THICKNESS, col));
            prev = glyph_path.at(i);
        }
        // make transparent
        sf::RectangleShape rect(sf::Vector2f(render_texture.getSize()));
        rect.setFillColor(sf::Color(255,255,255,64));
        render_texture.draw(rect, sf::RenderStates(sf::BlendMultiply));

        render_texture.display();
    }
};

#endif