#ifndef constants_hpp
#define constants_hpp

#include <random>
#include <unordered_map>
#include <array>
#include <set>
#include <SFML/Graphics.hpp>
#include "Vector.hpp"

const float pi = 3.1415;

const sf::Color Red = sf::Color(255, 0, 0.0);
const sf::Color Blue = sf::Color(0.0, 0, 255);
const sf::Color Green = sf::Color(0.0, 255, 0.0);
const sf::Color Yellow = sf::Color(128, 128, 0.0);
const sf::Color Grey = sf::Color(128, 128, 128);
const sf::Color LightGrey = sf::Color(200, 200, 200);
const sf::Color DarkGrey = sf::Color(5, 5, 5);
const sf::Color Transparent = sf::Color(0.0, 0, 0, 0.0);
const sf::Color HalfTransparent = sf::Color(255, 255, 255, 50);

const sf::Color White = sf::Color(255, 255, 255);
const sf::Color Brown = sf::Color(139, 69, 19);

const sf::Color AIR_COLOUR = sf::Color(230, 230, 230);
const sf::Color FIRE_COLOUR = sf::Color(128, 0, 0);
const sf::Color WATER_COLOUR = sf::Color(0, 0, 128);
const sf::Color EARTH_COLOUR = sf::Color(0, 128, 0);
const sf::Color LIFE_COLOUR = sf::Color(255, 212, 42);
const sf::Color DEATH_COLOUR = Grey;//sf::Color(0, 0, 0);

const float RADIANS_TO_DEGREES = 180/pi;
const float DEGREES_TO_RADIANS = pi/180;

const float SQRT3 = 1.7320;

const int WIDTH = 1920;
const int HEIGHT = 1080;

const double HAND_DRIFT = 50;

const int NUM_FINGERS = 5;

const int MAX_FRAMES = 9;
const double FINGER_FRAME_TIME = 1.0/40.0;
const double GESTURE_FRAME_TIME = 1.0/20.0;
const double GLYPH_DRAW_TIME = 2.5;

const Vector BASE_POS(WIDTH/2, HEIGHT-150);
const Vector RIGHTHAND_POS(150, 0);
const Vector LEFTHAND_POS(-150, 0);
const Vector GLYPH_POS(0, -250);
const double GLYPH_SCALE = 100.0;
const double GLYPH_MOVEMENT_SCALE = 100.0;
const double GLYPH_THICKNESS = 10.0;

enum class FingerState {Opening, Open, Closing, Closed};
enum class HandState {None, WindUp, Hold, CastReady, Casting, WindDown, Cancel};
enum class Gesture {None, Air, Fire, Earth, Water, Life, Death, Vert, Horz, Cast, Rotate};
enum class HandDataState {None, Wandering, Holding, Moving, Drawing, Modifying, Casting};

// Zac why is your compiler dumb?
namespace std {
    template <> struct hash<Gesture>
    {
        size_t operator()(const Gesture & g) const
        {
            return size_t(g);
        }
    };
}

enum class InterpType {Linear, CubicBezier, SineBezier};
const Vector BEZIERSTART = Vector(0.0,  0.0);
const Vector BEZIEREND = Vector(1.0,  1.0);

const Vector CUBICBEZIER1 = Vector(0.85, 0.0);
const Vector CUBICBEZIER2 = Vector(0.15, 1.0);

const Vector SINEBEZIER1 = Vector(0.5,  0.0);
const Vector SINEBEZIER2 = Vector(0.7, 0.7);

const double GLYPH_ROTATION_TIME = 9*GESTURE_FRAME_TIME;
const double GLYPH_TRANSLATE_TIME = GLYPH_ROTATION_TIME;

const std::vector<Vector> EARTHSQUARE = {
    Vector(-1, 1),
    Vector(1, 1),
    Vector(1, -1),
    Vector(-1, -1),
    Vector(-1, 1),
};

const std::vector<Vector> WATERCIRCLE = {
    Vector(-0.7071, 0.7071),
    Vector(-0.5555, 0.8314),
    Vector(-0.3826, 0.9238),
    Vector(-0.1950, 0.9807),
    Vector(0.0, 1.0),
    Vector(0.1950, 0.9807),
    Vector(0.3826, 0.9238),
    Vector(0.5555, 0.8314),
    Vector(0.7071, 0.7071),
    Vector(0.8314, 0.5555),
    Vector(0.9238, 0.3826),
    Vector(0.9807, 0.1950),
    Vector(1.0, 0.0),
    Vector(0.9807, -0.1950),
    Vector(0.9238, -0.3826),
    Vector(0.8314, -0.5555),
    Vector(0.7071, -0.7071),
    Vector(0.5555, -0.8314),
    Vector(0.3826, -0.9238),
    Vector(0.1950, -0.9807),
    Vector(0.0, -1.0),
    Vector(-0.1950, -0.9807),
    Vector(-0.3826, -0.9238),
    Vector(-0.5555, -0.8314),
    Vector(-0.7071, -0.7071),
    Vector(-0.8314, -0.5555),
    Vector(-0.9238, -0.3826),
    Vector(-0.9807, -0.1950),
    Vector(-1.0, 0.0),
    Vector(-0.9807, 0.1950),
    Vector(-0.9238, 0.3826),
    Vector(-0.8314, 0.5555),
    Vector(-0.7071, 0.7071),
};

const std::vector<Vector> FIRETRIANGLE = {
    Vector(-0.8660, 0.5),
    Vector(0.0, -1.0),
    Vector(0.8660, 0.5),
    Vector(-0.8660, 0.5),
};

const std::vector<Vector> AIRLINES = {
    Vector(-0.9955, -0.4203),
    Vector(-0.9707, -0.5170),
    Vector(-0.9275, -0.6071),
    Vector(-0.8676, -0.6871),
    Vector(-0.7932, -0.7538),
    Vector(-0.7072, -0.8047),
    Vector(-0.6130, -0.8378),
    Vector(-0.5141, -0.8519),
    Vector(-0.4143, -0.8465),
    Vector(-0.3175, -0.8217),
    Vector(-0.2274, -0.7785),
    Vector(-0.1475, -0.7185),
    Vector(-0.0808, -0.6442),
    Vector(-0.0299, -0.5582),
    Vector(0.0308, -0.3912),
    Vector(0.0817, -0.3053),
    Vector(0.1484, -0.2309),
    Vector(0.2283, -0.1709),
    Vector(0.3184, -0.1277),
    Vector(0.4152, -0.1029),
    Vector(0.5149, -0.0975),
    Vector(0.6138, -0.1116),
    Vector(0.7081, -0.1448),
    Vector(0.7941, -0.1957),
    Vector(0.8684, -0.2624),
    Vector(0.9284, -0.3423),
    Vector(0.9716, -0.4324),
    Vector(0.9964, -0.5292),
    Vector(0.9955, 0.4203),    
    Vector(0.9707, 0.5170),
    Vector(0.9275, 0.6071),
    Vector(0.8676, 0.6871),
    Vector(0.7932, 0.7538),
    Vector(0.7072, 0.8047),
    Vector(0.6130, 0.8378),
    Vector(0.5141, 0.8519),
    Vector(0.4143, 0.8465),
    Vector(0.3175, 0.8217),
    Vector(0.2274, 0.7785),
    Vector(0.1475, 0.7185),
    Vector(0.0808, 0.6442),
    Vector(0.0299, 0.5582),
    Vector(-0.0308, 0.3912),
    Vector(-0.0817, 0.3053),
    Vector(-0.1484, 0.2309),
    Vector(-0.2283, 0.1709),
    Vector(-0.3184, 0.1277),
    Vector(-0.4152, 0.1029),
    Vector(-0.5149, 0.0975),
    Vector(-0.6138, 0.1116),
    Vector(-0.7081, 0.1448),
    Vector(-0.7941, 0.1957),
    Vector(-0.8684, 0.2624),
    Vector(-0.9284, 0.3423),
    Vector(-0.9716, 0.4324),
    Vector(-0.9964, 0.5292),
};

const std::vector<Vector> AIRLINES_MOVE = {
    Vector(-0.9955, -0.4203),
    Vector(-0.9707, -0.5170),
    Vector(-0.9275, -0.6071),
    Vector(-0.8676, -0.6871),
    Vector(-0.7932, -0.7538),
    Vector(-0.7072, -0.8047),
    Vector(-0.6130, -0.8378),
    Vector(-0.5141, -0.8519),
    Vector(-0.4143, -0.8465),
    Vector(-0.3175, -0.8217),
    Vector(-0.2274, -0.7785),
    Vector(-0.1475, -0.7185),
    Vector(-0.0808, -0.6442),
    Vector(-0.0299, -0.5582),
    Vector(0.0308, -0.3912),
    Vector(0.0817, -0.3053),
    Vector(0.1484, -0.2309),
    Vector(0.2283, -0.1709),
    Vector(0.3184, -0.1277),
    Vector(0.4152, -0.1029),
    Vector(0.5149, -0.0975),
    Vector(0.6138, -0.1116),
    Vector(0.7081, -0.1448),
    Vector(0.7941, -0.1957),
    Vector(0.8684, -0.2624),
    Vector(0.9284, -0.3423),
    Vector(0.9716, -0.4324),
    Vector(0.9964, -0.5292),
    
    Vector(0.9964, -0.5292),
    Vector(0.9716, -0.4324),
    Vector(0.9284, -0.3423),
    Vector(0.8684, -0.2624),
    Vector(0.7941, -0.1957),
    Vector(0.7081, -0.1448),
    Vector(0.6138, -0.1116),
    Vector(0.5149, -0.0975),
    Vector(0.4152, -0.1029),
    Vector(0.3184, -0.1277),
    Vector(0.2283, -0.1709),
    Vector(0.1484, -0.2309),
    Vector(0.0817, -0.3053),
    Vector(0.0308, -0.3912),
    Vector(-0.0299, -0.5582),
    Vector(-0.0808, -0.6442),
    Vector(-0.1475, -0.7185),
    Vector(-0.2274, -0.7785),
    Vector(-0.3175, -0.8217),
    Vector(-0.4143, -0.8465),
    Vector(-0.5141, -0.8519),
    Vector(-0.6130, -0.8378),
    Vector(-0.7072, -0.8047),
    Vector(-0.7932, -0.7538),
    Vector(-0.8676, -0.6871),
    Vector(-0.9275, -0.6071),
    Vector(-0.9707, -0.5170),
    Vector(-0.9955, -0.4203),
};

const std::vector<Vector> DEATHCROSS = {
    Vector(-1, 1),
    Vector(1, -1),
    Vector(-1, -1),
    Vector(1, 1),
};

const std::vector<Vector> LIFESQUIGGLE = {
    Vector(0.0, 0.8230),
    Vector(-0.0881, 0.8954),
    Vector(-0.1888, 0.9492),
    Vector(-0.2979, 0.9823),
    Vector(-0.4115, 0.9935),
    Vector(-0.5250, 0.9823),
    Vector(-0.6342, 0.9492),
    Vector(-0.7348, 0.8954),
    Vector(-0.8230, 0.8230),
    Vector(-0.8954, 0.7348),
    Vector(-0.9492, 0.6342),
    Vector(-0.9823, 0.5250),
    Vector(-0.9935, 0.4115),
    Vector(-0.9823, 0.2979),
    Vector(-0.9492, 0.1888),
    Vector(-0.8954, 0.0881),
    Vector(-0.8230, 0.0),
    Vector(-0.7348, -0.0724),
    Vector(-0.6342, -0.1261),
    Vector(-0.5250, -0.1593),
    Vector(-0.4115, -0.1704),
    Vector(-0.2979, -0.1593),
    Vector(-0.1888, -0.1261),
    Vector(-0.0881, -0.0724),
    Vector(0.0, 0.0),
    Vector(0.0881, 0.0724),
    Vector(0.1888, 0.1261),
    Vector(0.2979, 0.1593),
    Vector(0.4115, 0.1704),
    Vector(0.5250, 0.1593),
    Vector(0.6342, 0.1261),
    Vector(0.7348, 0.0724),
    Vector(0.8230, 0.0),
    Vector(0.8954, -0.0881),
    Vector(0.9492, -0.1888),
    Vector(0.9823, -0.2979),
    Vector(0.9935, -0.4115),
    Vector(0.9823, -0.5250),
    Vector(0.9492, -0.6342),
    Vector(0.8954, -0.7348),
    Vector(0.8230, -0.8230),
    Vector(0.7348, -0.8954),
    Vector(0.6342, -0.9492),
    Vector(0.5250, -0.9823),
    Vector(0.4115, -0.9935),
    Vector(0.2979, -0.9823),
    Vector(0.1888, -0.9492),
    Vector(0.0881, -0.8954),
    Vector(0.0, -0.8230),
};

const std::set<Gesture> DRAWABLE_GESTURE = {
    Gesture::Air,
    Gesture::Fire,
    Gesture::Earth,
    Gesture::Water,
    Gesture::Life,
    Gesture::Death,
};

const std::set<Gesture> TWO_LINE_GESTURE = {
    Gesture::Air,
    Gesture::Death,
};

const std::unordered_map<Gesture, std::vector<Vector>> GESTURE_TO_MOVE = {
    {Gesture::Air, AIRLINES_MOVE},
    {Gesture::Fire, FIRETRIANGLE},
    {Gesture::Earth, EARTHSQUARE},
    {Gesture::Water, WATERCIRCLE},
    {Gesture::Life, LIFESQUIGGLE},
    {Gesture::Death, DEATHCROSS},
};

const std::unordered_map<Gesture, std::vector<Vector>> GESTURE_TO_GLYPH = {
    {Gesture::Air, AIRLINES},
    {Gesture::Fire, FIRETRIANGLE},
    {Gesture::Earth, EARTHSQUARE},
    {Gesture::Water, WATERCIRCLE},
    {Gesture::Life, LIFESQUIGGLE},
    {Gesture::Death, DEATHCROSS},
};

const std::unordered_map<Gesture, sf::Color> GESTURE_TO_COLOUR = {
    {Gesture::Air, AIR_COLOUR},
    {Gesture::Fire, FIRE_COLOUR},
    {Gesture::Earth, EARTH_COLOUR},
    {Gesture::Water, WATER_COLOUR},
    {Gesture::Life, LIFE_COLOUR},
    {Gesture::Death, DEATH_COLOUR},
};

const std::unordered_map<std::string, std::array<int, 6>> SpriteOffsets = {
    {"Air5", {0, 0, 121, 422, 218, 42}},
    {"Air4", {121, 0, 164, 419, 184, 43}},
    {"Air6", {285, 0, 183, 415, 166, 48}},
    {"Air7", {468, 0, 198, 410, 150, 51}},
    {"Rotate10", {666, 0, 229, 407, 164, 50}},
    {"Rotate9", {895, 0, 234, 405, 162, 51}},
    {"Air3", {1129, 0, 231, 404, 160, 52}},
    {"Rotate8", {1360, 0, 248, 397, 155, 56}},
    {"Up10", {1608, 0, 258, 395, 151, 31}},
    {"Up9", {1866, 0, 258, 394, 151, 32}},
    {"Up8", {2124, 0, 258, 392, 151, 36}},
    {"Cast7", {2382, 0, 302, 390, 130, 38}},
    {"Cast8", {2684, 0, 315, 384, 127, 47}},
    {"Rotate7", {2999, 0, 259, 383, 150, 63}},
    {"Horz10", {3258, 0, 274, 382, 49, 80}},
    {"Horz9", {3532, 0, 270, 382, 55, 78}},
    {"Horz8", {3802, 0, 260, 382, 75, 72}},
    {"Up7", {3258, 382, 258, 381, 151, 50}},
    {"Air2", {3516, 382, 261, 376, 149, 67}},
    {"Horz7", {3777, 382, 260, 375, 107, 70}},
    {"Air8", {2999, 383, 237, 374, 103, 71}},
    {"Down4", {2684, 384, 258, 370, 151, 62}},
    {"Cast9", {2124, 392, 321, 368, 126, 64}},
    {"Down3", {1866, 394, 258, 367, 151, 66}},
    {"Rotate6", {1360, 397, 260, 365, 149, 73}},
    {"Down5", {895, 405, 258, 365, 151, 68}},
    {"Up6", {468, 410, 258, 363, 151, 70}},
    {"Horz6", {121, 419, 259, 362, 141, 75}},
    {"Rotate4", {2445, 754, 258, 361, 150, 75}},
    {"Death3", {2703, 754, 293, 360, 53, 85}},
    {"Death2", {3777, 757, 267, 360, 126, 77}},
    {"Down2", {2996, 757, 258, 360, 151, 74}},
    {"Rotate3", {3516, 758, 258, 360, 150, 76}},
    {"Cast10", {1620, 761, 324, 359, 125, 74}},
    {"Water3", {2124, 760, 274, 359, 150, 76}},
    {"Horz4", {1153, 762, 274, 358, 154, 81}},
    {"Water2", {726, 770, 268, 358, 151, 77}},
    {"Life8", {1153, 404, 174, 358, 196, 109}},
    {"Fire6", {380, 773, 276, 357, 137, 77}},
    {"Fire5", {0, 781, 273, 357, 141, 77}},
    {"Fire4", {2703, 1114, 271, 357, 145, 77}},
    {"Horz3", {2398, 1115, 270, 357, 153, 80}},
    {"Fire3", {2974, 1117, 269, 357, 148, 77}},
    {"Horz5", {3243, 1117, 268, 357, 153, 80}},
    {"Fire2", {3774, 1117, 267, 357, 150, 77}},
    {"Horz2", {3511, 1118, 262, 357, 152, 78}},
    {"Rotate5", {1944, 1119, 258, 357, 151, 77}},
    {"Rotate2", {994, 1120, 258, 357, 151, 77}},
    {"Fire8", {1252, 1120, 280, 356, 130, 78}},
    {"Fire7", {1532, 1120, 279, 356, 133, 78}},
    {"Earth1", {656, 1128, 266, 356, 151, 78}},
    {"Fire1", {273, 1130, 266, 356, 151, 78}},
    {"Earth2", {0, 1138, 266, 356, 151, 78}},
    {"Water1", {2668, 1471, 266, 356, 151, 78}},
    {"Earth3", {2202, 1472, 263, 356, 151, 78}},
    {"Earth4", {2934, 1474, 259, 356, 151, 78}},
    {"Horz1", {3193, 1474, 258, 356, 151, 78}},
    {"Up1", {3773, 1474, 258, 356, 151, 78}},
    {"Air1", {3451, 1475, 258, 356, 151, 78}},
    {"Rotate1", {1252, 1476, 258, 356, 151, 78}},
    {"Down1", {1510, 1476, 258, 356, 151, 78}},
    {"Death1", {1768, 1476, 258, 356, 151, 78}},
    {"Earth5", {922, 1477, 253, 356, 151, 78}},
    {"Earth6", {539, 1484, 247, 356, 151, 78}},
    {"Earth7", {1620, 395, 241, 356, 151, 78}},
    {"Earth8", {2445, 390, 237, 356, 151, 78}},
    {"Earth9", {266, 1486, 234, 356, 151, 78}},
    {"Earth10", {0, 1494, 233, 356, 151, 78}},
    {"Life7", {2465, 1472, 199, 356, 181, 108}},
    {"Fire9", {2026, 1828, 282, 355, 128, 79}},
    {"Fire10", {2308, 1828, 282, 355, 128, 79}},
    {"Water4", {2590, 1828, 277, 355, 152, 80}},
    {"Life9", {726, 407, 156, 355, 206, 112}},
    {"Air9", {3258, 763, 243, 354, 96, 82}},
    {"Life10", {1944, 761, 150, 354, 209, 113}},
    {"Up2", {2867, 1830, 258, 353, 151, 82}},
    {"Down6", {3125, 1830, 258, 350, 151, 85}},
    {"Air10", {3709, 1830, 243, 349, 95, 85}},
    {"Up5", {3383, 1831, 258, 347, 151, 89}},
    {"Up3", {1175, 1832, 258, 345, 151, 91}},
    {"Life6", {1433, 1832, 248, 345, 164, 114}},
    {"Water5", {1681, 1832, 274, 341, 158, 93}},
    {"Up4", {786, 1833, 258, 340, 151, 96}},
    {"Cast6", {500, 1840, 283, 336, 137, 89}},
    {"Death4", {0, 1850, 289, 328, 4, 126}},
    {"Life5", {783, 2173, 290, 326, 151, 127}},
    {"Water6", {1681, 2173, 257, 324, 171, 107}},
    {"Death5", {289, 2176, 322, 293, 28, 162}},
    {"Death6", {1073, 2177, 322, 272, 48, 179}},
    {"Down7", {1395, 2177, 258, 322, 151, 116}},
    {"Water7", {0, 2178, 230, 313, 185, 113}},
    {"Water8", {289, 1842, 203, 310, 197, 117}},
    {"Water9", {1427, 762, 184, 309, 205, 121}},
    {"Water10", {2202, 1119, 178, 309, 207, 122}},
    {"Life4", {3383, 2178, 297, 306, 147, 140}},
    {"Cast5", {3680, 2179, 271, 299, 144, 127}},
    {"Death7", {1938, 2183, 297, 258, 72, 189}},
    {"Life3", {2235, 2183, 287, 290, 147, 149}},
    {"Down8", {3125, 2180, 258, 288, 151, 153}},
    {"Cast4", {2522, 2183, 263, 284, 148, 145}},
    {"Life2", {2785, 2183, 272, 282, 150, 154}},
    {"Life1", {1938, 2441, 266, 279, 151, 155}},
    {"Death8", {1073, 2449, 272, 254, 83, 191}},
    {"Cast3", {2785, 2465, 259, 265, 150, 167}},
    {"Down9", {2522, 2467, 258, 262, 151, 182}},
    {"Death9", {3044, 2468, 261, 252, 82, 192}},
    {"Down10", {230, 2469, 258, 252, 151, 192}},
    {"Death10", {488, 2469, 258, 252, 80, 192}},
    {"Cast2", {2204, 2473, 258, 251, 151, 183}},
    {"Cast1", {3680, 2478, 258, 245, 151, 189}},
    {"0Close2", {380, 415, 85, 214, 332, 155}},
    {"0Close1", {1073, 1833, 85, 214, 332, 155}},
    {"0Close3", {273, 781, 82, 211, 332, 158}},
    {"Palm", {3305, 2484, 210, 210, 151, 224}},
    {"0Close4", {1175, 1477, 76, 208, 332, 161}},
    {"0Close5", {611, 2176, 76, 204, 332, 165}},
    {"0Close6", {539, 1130, 76, 197, 331, 172}},
    {"0Close7", {0, 422, 75, 191, 331, 178}},
    {"0Close8", {1811, 1120, 77, 185, 331, 184}},
    {"0Close9", {786, 1484, 77, 181, 331, 188}},
    {"0Close10", {994, 770, 78, 180, 331, 189}},
    {"3Close1", {75, 422, 42, 168, 198, 78}},
    {"4Close1", {615, 1130, 41, 168, 151, 78}},
    {"2Close1", {1888, 1120, 41, 168, 246, 78}},
    {"1Close1", {863, 1484, 41, 168, 293, 78}},
    {"2Close2", {994, 950, 41, 165, 246, 82}},
    {"4Close2", {2398, 760, 41, 165, 151, 82}},
    {"1Close2", {2398, 925, 41, 165, 293, 82}},
    {"3Close2", {230, 2178, 42, 164, 198, 83}},
    {"1Close3", {1175, 1685, 41, 146, 293, 102}},
    {"4Close3", {687, 2176, 41, 146, 151, 102}},
    {"2Close3", {687, 2322, 41, 146, 246, 102}},
    {"3Close3", {380, 629, 42, 144, 198, 104}},
    {"1Close7", {422, 629, 41, 119, 293, 210}},
    {"4Close7", {230, 2342, 41, 119, 151, 210}},
    {"2Close7", {1073, 2047, 41, 119, 246, 210}},
    {"3Close7", {1114, 2047, 42, 118, 198, 210}},
    {"3Close8", {273, 992, 42, 118, 198, 211}},
    {"1Close8", {315, 992, 41, 118, 293, 211}},
    {"2Close8", {539, 1327, 41, 118, 246, 211}},
    {"4Close8", {580, 1327, 41, 118, 151, 211}},
    {"3Close9", {3057, 2183, 42, 117, 198, 212}},
    {"2Close9", {3057, 2300, 41, 117, 246, 212}},
    {"4Close9", {2942, 384, 41, 117, 151, 212}},
    {"1Close9", {2942, 501, 41, 117, 293, 212}},
    {"3Close10", {2942, 618, 42, 116, 198, 212}},
    {"4Close10", {0, 613, 41, 116, 151, 212}},
    {"2Close10", {41, 613, 41, 116, 246, 212}},
    {"1Close10", {3709, 1475, 41, 116, 293, 212}},
    {"2Close4", {3709, 1591, 41, 104, 246, 146}},
    {"4Close4", {3709, 1695, 41, 104, 151, 146}},
    {"1Close4", {1811, 1305, 41, 103, 293, 146}},
    {"3Close4", {1852, 1305, 42, 101, 198, 149}},
    {"2Close6", {1894, 1305, 41, 100, 246, 202}},
    {"1Close6", {786, 1665, 41, 100, 293, 202}},
    {"4Close6", {827, 1665, 41, 100, 151, 202}},
    {"3Close6", {868, 1665, 42, 98, 198, 202}},
    {"3Close5", {1894, 1405, 42, 71, 198, 182}},
    {"1Close5", {1852, 1406, 41, 70, 293, 181}},
    {"2Close5", {868, 1763, 41, 70, 246, 181}},
    {"4Close5", {611, 2380, 41, 70, 151, 181}},
};

// Sub Glyph Translation
enum class DIRECTION {LEFT, RIGHT, UP, DOWN};

class IndexDirection {
public:
    int index;
    DIRECTION direction;
    IndexDirection(int i, DIRECTION d)
    : index(i), direction(d)
    {}
    bool operator==(IndexDirection o) const
    {
        return index==o.index&&direction==o.direction;
    }
};

// reading order
// deathpoints same as earth
const std::vector<Vector> EARTHPOINTS = {
    Vector(-1, -1),
    Vector(0, -1),
    Vector(1, -1),
    Vector(-1, 0),
    Vector(0, 0),
    Vector(1, 0),
    Vector(-1, 1),
    Vector(0, 1),
    Vector(1, 1),
};

const std::vector<Vector> AIRPOINTS = {
    Vector(0, -1),
    Vector(0.5, -0.5),
    Vector(-0.5, -0.5),
    Vector(-1, 0),
    Vector(0, 0),
    Vector(1, 0),
    Vector(-0.5, 0.5),
    Vector(0.5, 0.5),
    Vector(0, 1),
};

const std::vector<Vector> FIREPOINTS = {
    Vector(0, -1),
    Vector(-0.8660, -0.5),
    Vector(0.8660, -0.5),
    Vector(-0.4330, -0.5),
    Vector(0, 0),
    Vector(0.4330, -0.5),
    Vector(-0.8660, 0.5),
    Vector(0, 0.5),
    Vector(0.8660, 0.5),
};

const std::vector<Vector> WATERPOINTS = {
    Vector(0.0, -1.0),
    Vector(-0.7071, -0.7071),
    Vector(0.7071, -0.7071),
    Vector(-1, 0),
    Vector(0, 0),
    Vector(1, 0),
    Vector(-0.7071, 0.7071),
    Vector(0.7071, 0.7071),
    Vector(0, 1),
};

const std::vector<Vector> LIFEPOINTS = {
    Vector(-0.8268, -0.8268),
    Vector(0, -0.8268),
    Vector(0.8268, -0.8268),
    Vector(-0.4134, -0.4134),
    Vector(0, 0),
    Vector(0.4134, 0.4134),
    Vector(-0.8268, 0.8268),
    Vector(0, 0.8268),
    Vector(0.8268, 0.8268),
};

// hashing junk
namespace std {
    template <> struct hash<DIRECTION>
    {
        size_t operator()(const DIRECTION & g) const
        {
            return size_t(g);
        }
    };
    template <> struct hash<IndexDirection>
    {
        size_t operator()(const IndexDirection & id) const
        {
            size_t const h1 ( std::hash<int>{}(id.index) );
            size_t const h2 ( std::hash<DIRECTION>{}(id.direction) );
            return h1 ^ (h2 << 1);
        }
    };
}

// direction to point sub-glyph position index
const std::unordered_map<IndexDirection, int> EARTHPOINTTRANSLATE = {
    {IndexDirection(1, DIRECTION::LEFT), 0},
    {IndexDirection(2, DIRECTION::LEFT), 1},
    {IndexDirection(4, DIRECTION::LEFT), 3},
    {IndexDirection(5, DIRECTION::LEFT), 4},
    {IndexDirection(7, DIRECTION::LEFT), 6},
    {IndexDirection(8, DIRECTION::LEFT), 7},
    
    {IndexDirection(0, DIRECTION::RIGHT), 1},
    {IndexDirection(1, DIRECTION::RIGHT), 2},
    {IndexDirection(3, DIRECTION::RIGHT), 4},
    {IndexDirection(4, DIRECTION::RIGHT), 5},
    {IndexDirection(6, DIRECTION::RIGHT), 7},
    {IndexDirection(7, DIRECTION::RIGHT), 8},

    {IndexDirection(3, DIRECTION::UP), 0},
    {IndexDirection(4, DIRECTION::UP), 1},
    {IndexDirection(5, DIRECTION::UP), 2},
    {IndexDirection(6, DIRECTION::UP), 3},
    {IndexDirection(7, DIRECTION::UP), 4},
    {IndexDirection(8, DIRECTION::UP), 5},

    {IndexDirection(0, DIRECTION::DOWN), 3},
    {IndexDirection(1, DIRECTION::DOWN), 4},
    {IndexDirection(2, DIRECTION::DOWN), 5},
    {IndexDirection(3, DIRECTION::DOWN), 6},
    {IndexDirection(4, DIRECTION::DOWN), 7},
    {IndexDirection(5, DIRECTION::DOWN), 8},
};

const std::unordered_map<IndexDirection, int> AIRPOINTTRANSLATE = {
    {IndexDirection(0, DIRECTION::LEFT), 1},
    {IndexDirection(1, DIRECTION::LEFT), 3},
    {IndexDirection(2, DIRECTION::LEFT), 1},
    {IndexDirection(4, DIRECTION::LEFT), 3},
    {IndexDirection(5, DIRECTION::LEFT), 4},
    {IndexDirection(6, DIRECTION::LEFT), 3},
    {IndexDirection(7, DIRECTION::LEFT), 6},
    {IndexDirection(8, DIRECTION::LEFT), 6},

    {IndexDirection(0, DIRECTION::RIGHT), 2},
    {IndexDirection(1, DIRECTION::RIGHT), 2},
    {IndexDirection(2, DIRECTION::RIGHT), 5},
    {IndexDirection(3, DIRECTION::RIGHT), 4},
    {IndexDirection(4, DIRECTION::RIGHT), 5},
    {IndexDirection(6, DIRECTION::RIGHT), 7},
    {IndexDirection(7, DIRECTION::RIGHT), 5},
    {IndexDirection(8, DIRECTION::RIGHT), 7},

    {IndexDirection(1, DIRECTION::UP), 0},
    {IndexDirection(2, DIRECTION::UP), 0},
    {IndexDirection(3, DIRECTION::UP), 1},
    {IndexDirection(4, DIRECTION::UP), 0},
    {IndexDirection(5, DIRECTION::UP), 2},
    {IndexDirection(6, DIRECTION::UP), 1},
    {IndexDirection(7, DIRECTION::UP), 2},
    {IndexDirection(8, DIRECTION::UP), 4},

    {IndexDirection(0, DIRECTION::DOWN), 4},
    {IndexDirection(1, DIRECTION::DOWN), 6},
    {IndexDirection(2, DIRECTION::DOWN), 7},
    {IndexDirection(3, DIRECTION::DOWN), 6},
    {IndexDirection(4, DIRECTION::DOWN), 8},
    {IndexDirection(5, DIRECTION::DOWN), 7},
    {IndexDirection(6, DIRECTION::DOWN), 8},
    {IndexDirection(7, DIRECTION::DOWN), 8},
};

const std::unordered_map<IndexDirection, int> FIREPOINTTRANSLATE = {
    {IndexDirection(0, DIRECTION::LEFT), 1},
    {IndexDirection(2, DIRECTION::LEFT), 5},
    {IndexDirection(3, DIRECTION::LEFT), 1},
    {IndexDirection(4, DIRECTION::LEFT), 3},
    {IndexDirection(5, DIRECTION::LEFT), 4},
    {IndexDirection(7, DIRECTION::LEFT), 6},
    {IndexDirection(8, DIRECTION::LEFT), 7},

    {IndexDirection(0, DIRECTION::RIGHT), 2},
    {IndexDirection(1, DIRECTION::RIGHT), 3},
    {IndexDirection(3, DIRECTION::RIGHT), 4},
    {IndexDirection(4, DIRECTION::RIGHT), 5},
    {IndexDirection(5, DIRECTION::RIGHT), 2},
    {IndexDirection(6, DIRECTION::RIGHT), 7},
    {IndexDirection(7, DIRECTION::RIGHT), 8},

    {IndexDirection(1, DIRECTION::UP), 0},
    {IndexDirection(2, DIRECTION::UP), 0},
    {IndexDirection(3, DIRECTION::UP), 0},
    {IndexDirection(4, DIRECTION::UP), 0},
    {IndexDirection(5, DIRECTION::UP), 0},
    {IndexDirection(6, DIRECTION::UP), 3},
    {IndexDirection(7, DIRECTION::UP), 4},
    {IndexDirection(8, DIRECTION::UP), 5},

    {IndexDirection(0, DIRECTION::DOWN), 4},
    {IndexDirection(1, DIRECTION::DOWN), 6},
    {IndexDirection(2, DIRECTION::DOWN), 8},
    {IndexDirection(3, DIRECTION::DOWN), 6},
    {IndexDirection(4, DIRECTION::DOWN), 7},
    {IndexDirection(5, DIRECTION::DOWN), 8},
};

const std::unordered_map<IndexDirection, int> WATERPOINTTRANSLATE = {
    {IndexDirection(0, DIRECTION::LEFT), 1},
    {IndexDirection(1, DIRECTION::LEFT), 3},
    {IndexDirection(2, DIRECTION::LEFT), 1},
    {IndexDirection(4, DIRECTION::LEFT), 3},
    {IndexDirection(5, DIRECTION::LEFT), 4},
    {IndexDirection(6, DIRECTION::LEFT), 3},
    {IndexDirection(7, DIRECTION::LEFT), 8},
    {IndexDirection(8, DIRECTION::LEFT), 6},

    {IndexDirection(0, DIRECTION::RIGHT), 2},
    {IndexDirection(1, DIRECTION::RIGHT), 0},
    {IndexDirection(2, DIRECTION::RIGHT), 5},
    {IndexDirection(3, DIRECTION::RIGHT), 4},
    {IndexDirection(4, DIRECTION::RIGHT), 5},
    {IndexDirection(6, DIRECTION::RIGHT), 8},
    {IndexDirection(7, DIRECTION::RIGHT), 5},
    {IndexDirection(8, DIRECTION::RIGHT), 7},
    
    {IndexDirection(1, DIRECTION::UP), 0},
    {IndexDirection(2, DIRECTION::UP), 0},
    {IndexDirection(3, DIRECTION::UP), 1},
    {IndexDirection(4, DIRECTION::UP), 0},
    {IndexDirection(5, DIRECTION::UP), 2},
    {IndexDirection(6, DIRECTION::UP), 3},
    {IndexDirection(7, DIRECTION::UP), 5},
    {IndexDirection(8, DIRECTION::UP), 4},

    {IndexDirection(0, DIRECTION::DOWN), 4},
    {IndexDirection(1, DIRECTION::DOWN), 3},
    {IndexDirection(2, DIRECTION::DOWN), 5},
    {IndexDirection(3, DIRECTION::DOWN), 6},
    {IndexDirection(4, DIRECTION::DOWN), 8},
    {IndexDirection(5, DIRECTION::DOWN), 7},
    {IndexDirection(6, DIRECTION::DOWN), 8},
    {IndexDirection(7, DIRECTION::DOWN), 8},
};

const std::unordered_map<IndexDirection, int> LIFEPOINTTRANSLATE = {
    {IndexDirection(1, DIRECTION::LEFT), 0},
    {IndexDirection(2, DIRECTION::LEFT), 1},
    {IndexDirection(3, DIRECTION::LEFT), 0},
    {IndexDirection(4, DIRECTION::LEFT), 3},
    {IndexDirection(5, DIRECTION::LEFT), 4},
    {IndexDirection(7, DIRECTION::LEFT), 6},
    {IndexDirection(8, DIRECTION::LEFT), 7},

    {IndexDirection(0, DIRECTION::RIGHT), 1},
    {IndexDirection(1, DIRECTION::RIGHT), 2},
    {IndexDirection(3, DIRECTION::RIGHT), 4},
    {IndexDirection(4, DIRECTION::RIGHT), 5},
    {IndexDirection(5, DIRECTION::RIGHT), 8},
    {IndexDirection(6, DIRECTION::RIGHT), 7},
    {IndexDirection(7, DIRECTION::RIGHT), 8},

    {IndexDirection(3, DIRECTION::UP), 1},
    {IndexDirection(4, DIRECTION::UP), 1},
    {IndexDirection(5, DIRECTION::UP), 4},
    {IndexDirection(6, DIRECTION::UP), 0},
    {IndexDirection(7, DIRECTION::UP), 4},
    {IndexDirection(8, DIRECTION::UP), 5},

    {IndexDirection(0, DIRECTION::DOWN), 6},
    {IndexDirection(1, DIRECTION::DOWN), 4},
    {IndexDirection(2, DIRECTION::DOWN), 8},
    {IndexDirection(3, DIRECTION::DOWN), 4},
    {IndexDirection(4, DIRECTION::DOWN), 7},
    {IndexDirection(5, DIRECTION::DOWN), 7},
};

#endif