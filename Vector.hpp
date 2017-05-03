#ifndef vector_hpp
#define vector_hpp

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

class Vector
{
public:
    float x,y;
    Vector() : x(0), y(0) {}
    Vector(float x, float y) : x(x), y(y) {}
    Vector(double x, double y) : x(x), y(y) {}
    Vector(int x, int y) : x((float)x), y((float)y) {}
    Vector(sf::Vector2f v) : x(v.x), y(v.y) {}
    Vector(sf::Vector2i v) : x((float)v.x), y((float)v.y) {}
    Vector operator-(Vector o) const
    {
        return Vector(x-o.x, y-o.y);
    }
    Vector operator-=(Vector o)
    {
        x -= o.x;
        y -= o.y;
        return *this;
    }
    bool operator==(Vector o) const
    {
        return x==o.x&&y==o.y;
    }
    bool operator!=(Vector o) const
    {
        return !(*this == o);
    }
    Vector operator+(Vector o) const
    {
        return Vector(x+o.x, y+o.y);
    }
    Vector operator+=(Vector o)
    {
        x += o.x;
        y += o.y;
        return *this;
    }
    Vector operator*(float m) const
    {
        return Vector(m*x, m*y);
    }
    Vector operator*=(float m)
    {
        x *= m;
        y *= m;
        return *this;
    }    
    Vector operator/(float m) const
    {
        return Vector(x/m, y/m);
    }
    Vector operator/=(float m)
    {
        x /= m;
        y /= m;
        return *this;
    }
    float magnitude()
    {
        return sqrt(x*x + y*y);
    }
    float magnitude_squared()
    {
        return x*x + y*y;
    }
    Vector normalise()
    {
        float m = magnitude();
        return (*this)/m;
    }
    float dot(Vector other)
    {
        return x*other.x + y*other.y;
    }
    Vector projection(Vector other)
    {
        return other * dot(other)/other.dot(other);
    }
    sf::Vector2f to_sfml() const
    {
        return sf::Vector2f(x, y);
    }
};

namespace std {
    template <> struct hash<Vector>
    {
        size_t operator()(const Vector & v) const
        {
            size_t const h1 ( std::hash<double>{}(v.x) );
            size_t const h2 ( std::hash<double>{}(v.y) );
            return h1 ^ (h2 << 1);
        }
    };
}

#endif