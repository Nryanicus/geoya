#ifndef vector_hpp
#define vector_hpp

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

class Vector
{
public:
    double x,y;
    Vector() : x(0), y(0) {}
    Vector(double x, double y) : x(x), y(y) {}
    Vector(int x, double y) : x((double)x), y((double)y) {}
    Vector(double x, int y) : x((double)x), y((double)y) {}
    Vector(int x, int y) : x((double)x), y((double)y) {}
    Vector(sf::Vector2f v) : x(v.x), y(v.y) {}
    Vector(sf::Vector2i v) : x((double)v.x), y((double)v.y) {}
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
    Vector operator*(double m) const
    {
        return Vector(m*x, m*y);
    }
    Vector operator*=(double m)
    {
        x *= m;
        y *= m;
        return *this;
    }    
    Vector operator/(double m) const
    {
        return Vector(x/m, y/m);
    }
    Vector operator/=(double m)
    {
        x /= m;
        y /= m;
        return *this;
    }
    double magnitude()
    {
        return sqrt(x*x + y*y);
    }
    double magnitude_squared()
    {
        return x*x + y*y;
    }
    Vector normalise()
    {
        double m = magnitude();
        return (*this)/m;
    }
    double dot(Vector other)
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