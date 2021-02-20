#include <SFML/Graphics.hpp>


const int size = 18;    // texture size
int random = time(NULL);

bool started = false;
bool paused = false;
int brick_amount = 0;

bool holdInitialized = false;
bool initialized = false;

const int score_amount = 10;
const int score_name_size = 10;
const int level_amount = 11;

const int fps = 100;

const sf::Color darker_color = { 200, 200, 200, 125 };


class { public:
    const int x1 = -2 * size;
    const int x2 = 8 * size;
    const int y1 = 0;
    const int y2 = 40 * size;
} const boundaries;
              
class { public:
    const int x = 10;
    const int y = 40;
    const int area = x * y;
} const field_size;

struct Previous {
      int x;
      int y;
      int colorId;
};
Previous previous[400];   // field_size.area

class { public:
    float init_speed = 1.2f * fps;
    float speed = init_speed; // step [Hz] = 100 / fallSpeed
    int counter = 0;
public:
    void increaseSpeed(int times = 1) {
        for (int i = 0; i < times; i++)
            speed *= 0.85f;
    }
} fall;

class { public:
    sf::Vector2f points{ 11 * size, 5 * size };
    sf::Vector2f current { 2 * size, 2 * size };
    sf::Vector2f hold { 11 * size, 16 * size };
    sf::Vector2f next[3] {
        {11 * size, 24 * size},
        {11 * size, 31 * size},
        {11 * size, 38 * size}
    };
} const render_positions;

const sf::Vector2f window_size { 540, 1000 };

const sf::Vector2f field_offset{
    (window_size.x - size * 12) / 2,
    (window_size.y - size * 42) / 2
};


sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "TETRIS", sf::Style::Default);
sf::Texture texture;
sf::Font font;
