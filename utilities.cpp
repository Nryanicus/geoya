#include "utilities.hpp"

std::default_random_engine RandomGenerator(time(NULL));

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    os << "V: " << v.x << ", " << v.y;
    return os;
}

//////////////////////
/*      Hex Ops     */
//////////////////////

std::pair<int, int> round_hex(float q, float r)
{
    float x = q;
    float z = r;
    float y = -x-z;

    int rx = (int) round(x);
    int ry = (int) round(y);
    int rz = (int) round(z);

    int x_diff = abs(rx - x);
    int y_diff = abs(ry - y);
    int z_diff = abs(rz - z);

    if ((x_diff > y_diff) && (x_diff > z_diff))
        rx = -ry-rz;
    else if (y_diff > z_diff)
        ry = -rx-rz;
    else
        rz - -rx-ry;

    return std::make_pair(rx, rz);
}

std::pair<int, int> pixel_to_axial(float x, float y)
{
    float q = x*SQRT3/3.0 - y/3.0;
    float r = y*2/3.0;
    return round_hex(q, r);
}

std::pair<float, float> axial_to_pixel(float q, float r)
{
    float x = SQRT3*(q+r/2.0);
    float y = 3/2.0*r;
    return std::make_pair(x, y);
}

//////////////////////
/* SFML Vector Ops */
//////////////////////

sf::Vector2f rotate(float x, float y, float theta)
{
    float x_p = cos(theta)*x - sin(theta)*y;
    float y_p = sin(theta)*x + cos(theta)*y;
    return sf::Vector2f(x_p, y_p);
}

float magnitude(sf::Vector2f* v)
{
    return sqrt(v->x*v->x + v->y*v->y);
}

sf::Vector2f normalise(sf::Vector2f* v)
{
    float m = magnitude(v);
    return sf::Vector2f(v->x/m, v->y/m);
}

float dot_product(sf::Vector2f* v1, sf::Vector2f* v2)
{
    return v1->x*v2->x + v1->y*v2->y;
}

// in degrees
float angle_between(sf::Vector2f* v1, sf::Vector2f* v2)
{
    sf::Vector2f v_1 = normalise(v1);
    sf::Vector2f v_2 = normalise(v2);
    return acos(dot_product(&v_1, &v_2)) * RADIANS_TO_DEGREES;
}

//////////////////////
/* Box2D Vector Ops */
//////////////////////

// b2Vec2 rotate(b2Vec2* vec, float theta)
// {
//     float x_p = cos(theta)*vec->x - sin(theta)*vec->y;
//     float y_p = sin(theta)*vec->x + cos(theta)*vec->y;
//     return b2Vec2(x_p, y_p);
// }

// float magnitude(b2Vec2* v)
// {
//     return sqrt(v->x*v->x + v->y*v->y);
// }

// b2Vec2 normalise(b2Vec2* v)
// {
//     float m = magnitude(v);
//     return b2Vec2(v->x/m, v->y/m);
// }

// b2Vec2  project(b2Vec2* v1, b2Vec2* v2)
// {
//     float m = b2Dot(*v1, *v2) / v2->LengthSquared();
//     b2Vec2 vec = *v2;
//     vec *= m;
//     return vec;
// }

// // in radians
// float angle_between(b2Vec2* v1, b2Vec2* v2)
// {
//     b2Vec2 v_1 = normalise(v1);
//     b2Vec2 v_2 = normalise(v2);
//     return acos(b2Dot(v_1, v_2));
// }

//////////////////////
/*     Misc Ops     */
//////////////////////

/* return random int in range [a, b], assumes srand already called*/
int random_int(int a, int b)
{
    if (a==b) return a;
    assert (a<b);
    std::uniform_int_distribution<int> distr(a, b);
    int c = distr(RandomGenerator);
    assert (c>=a && c<=b);
    return c;
}

sf::IntRect spritesheet_rect(std::string sprite, std::string frame)
{
    // std::cout << "rect" << std::endl;
    // std::cout << sprite << std::endl;
    // std::cout << frame << std::endl;
    if (sprite == "TopBack")
        return sf::IntRect(TopBackSpriteOffsets.at(sprite+frame).at(0), TopBackSpriteOffsets.at(sprite+frame).at(1), TopBackSpriteOffsets.at(sprite+frame).at(2), TopBackSpriteOffsets.at(sprite+frame).at(3));
    else if (sprite == "Front")
        return sf::IntRect(FrontSpriteOffsets.at(sprite+frame).at(0), FrontSpriteOffsets.at(sprite+frame).at(1), FrontSpriteOffsets.at(sprite+frame).at(2), FrontSpriteOffsets.at(sprite+frame).at(3));
    else
    {
        assert (false);
        return sf::IntRect(0,0,0,0);
    }
}

sf::Vector2f spritesheet_offset(std::string sprite, std::string frame)
{
    if (sprite == "TopBack")
        return sf::Vector2f(TopBackSpriteOffsets.at(sprite+frame).at(4), TopBackSpriteOffsets.at(sprite+frame).at(5));
    else if (sprite == "Front")
        return sf::Vector2f(FrontSpriteOffsets.at(sprite+frame).at(4), FrontSpriteOffsets.at(sprite+frame).at(5));
    else
    {
        assert (false);
        return sf::Vector2f(0, 0);
    }
}