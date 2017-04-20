#ifndef utilities_hpp
#define utilities_hpp

#include <SFML/Graphics.hpp>
// #include <Box2D/Box2D.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "Vector.hpp"
#include "constants.hpp"

std::ostream& operator<<(std::ostream& os, const Vector& v);

//////////////////////
/*      Hex Ops     */
//////////////////////

std::pair<int, int> round_hex(float q, float r);

std::pair<int, int> pixel_to_axial(float x, float y);

std::pair<float, float> axial_to_pixel(float q, float r);

//////////////////////
/* SFML Vector Ops */
//////////////////////

sf::Vector2f rotate(float x, float y, float theta);

float magnitude(sf::Vector2f* v);

sf::Vector2f normalise(sf::Vector2f* v);

float dot_product(sf::Vector2f* v1, sf::Vector2f* v2);

// in degrees
float angle_between(sf::Vector2f* v1, sf::Vector2f* v2);

sf::RectangleShape line_with_thickness(sf::Vector2f p1, sf::Vector2f p2, float thickness, int trans1=255, int trans2=255);

//////////////////////
/* Box2D Vector Ops */
//////////////////////

// b2Vec2 rotate(b2Vec2* vec, float theta);

// float magnitude(b2Vec2* v);

// b2Vec2 normalise(b2Vec2* v);

// b2Vec2  project(b2Vec2* v1, b2Vec2* v2);

// // in radians
// float angle_between(b2Vec2* v1, b2Vec2* v2);


//////////////////////
/*     Misc Ops     */
//////////////////////

int random_int(int a, int b);

sf::IntRect spritesheet_rect(std::string sprite, std::string frame);

sf::Vector2f spritesheet_offset(std::string sprite, std::string frame);

#endif