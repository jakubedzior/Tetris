#include <random>

class BrickSprites
{
private:
	sf::Sprite brick;
public:
	BrickSprites(int shapeId) {
		brick.setTexture(texture);
		refreshTextures(shapeId);
	}
	void refreshTextures(int shapeId) {
		brick.setTextureRect(sf::IntRect(((shapeId + random) % 7) * size, 0, size, size));
	}
	void draw(int i) {
		brick.setPosition(previous[i].x + field_offset.x, previous[i].y + field_offset.y);
		window.draw(brick);
	}
	void drawAt(int pos_x, int pos_y, double scale = 1.0) {
		sf::Sprite temp = brick;
		temp.setPosition(pos_x, pos_y);
		temp.setScale(sf::Vector2f(scale, scale));
		window.draw(temp);

	}
};

BrickSprites bricks[7] { 0, 1, 2, 3, 4, 5, 6 };
