class Title
{
private:
	sf::Vector2f position = {0, 0};
	int tetris[6][5][3] = {
		{ {1, 1, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
		{ {1, 1, 1}, {1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {1, 1, 1}},
		{ {1, 1, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
		{ {1, 1, 0}, {1, 0, 1}, {1, 1, 0}, {1, 0, 1}, {1, 0, 1}},
		{ {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
		{ {1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}}
	};
	int colorIds[90];

public:
	bool initialized = false;

	Title() {
		tryScrambling();
	}
	void tryScrambling() {
		if (!initialized) {
			for (int i = 0; i < 90; i++) {
				colorIds[i] = std::rand() % 7;
			}
			initialized = true;
		}
	}
	void drawHorizontally(int pos_x, int pos_y) {
		for (int letter = 0; letter < 6; letter++) {
			for (int row = 0; row < 5; row++) {
				for (int col = 0; col < 3; col++) {
					if (tetris[letter][row][col] == 1)
						bricks[colorIds[col+row*3+letter*15]].drawAt(pos_x+letter*size*3.5+col*size, pos_y+row*size);
				}
			}
		}
	}
	void drawVertically(int pos_x, int pos_y) {
		for (int letter = 0; letter < 6; letter++) {
			for (int row = 0; row < 5; row++) {
				for (int col = 0; col < 3; col++) {
					if (tetris[letter][row][col] == 1)
						bricks[colorIds[col + row * 3 + letter * 15]].drawAt(pos_x + col * size, pos_y + letter * size * 6 + row * size);
				}
			}
		}
	}
	void drawObliquely(int pos_x, int pos_y, double scale = 1.0) {
		for (int letter = 0; letter < 6; letter++) {
			for (int row = 0; row < 5; row++) {
				for (int col = 0; col < 3; col++) {
					if (tetris[letter][row][col] == 1)
						bricks[colorIds[col + row * 3 + letter * 15]].drawAt(pos_x + col * size * scale + letter * 2.5 * size * scale, pos_y + letter * size * 2.5 * scale + row * size * scale, scale);
				}
			}
		}
	}
};

Title title;

