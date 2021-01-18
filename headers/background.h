class Background {
private:
	const float scale = 0.296f;

	sf::Sprite hook_start;
	sf::Sprite hook_middle;
	sf::Sprite hook_end;
	sf::Sprite left;
	sf::Sprite right;
	sf::Sprite corner_left;
	sf::Sprite corner_right;
	sf::Sprite bottom;

	sf::Texture hook_start_texture;
	sf::Texture hook_middle_texture;
	sf::Texture hook_end_texture;
	sf::Texture left_texture;
	sf::Texture right_texture;
	sf::Texture corner_left_texture;
	sf::Texture corner_right_texture;
	sf::Texture bottom_texture;

public:
	bool lines = false;

	Background() {
		hook_start_texture.loadFromFile("background/hook_start.png");
		hook_start.setTexture(hook_start_texture);
		hook_start.setScale(sf::Vector2f(scale, scale));

		hook_middle_texture.loadFromFile("background/hook_middle.png");
		hook_middle.setTexture(hook_middle_texture);
		hook_middle.setScale(sf::Vector2f(scale, scale));

		hook_end_texture.loadFromFile("background/hook_end.png");
		hook_end.setTexture(hook_end_texture);
		hook_end.setScale(sf::Vector2f(scale, scale));

		left_texture.loadFromFile("background/left.png");
		left.setTexture(left_texture);
		left.setScale(sf::Vector2f(scale, scale));

		right_texture.loadFromFile("background/right.png");
		right.setTexture(right_texture);
		right.setScale(sf::Vector2f(scale, scale));

		corner_left_texture.loadFromFile("background/corner_left.png");
		corner_left.setTexture(corner_left_texture);
		corner_left.setScale(sf::Vector2f(scale, scale));

		corner_right_texture.loadFromFile("background/corner_right.png");
		corner_right.setTexture(corner_right_texture);
		corner_right.setScale(sf::Vector2f(scale, scale));

		bottom_texture.loadFromFile("background/bottom.png");
		bottom.setTexture(bottom_texture);
		bottom.setScale(sf::Vector2f(scale, scale));
	}
	void drawEmpty() {
		hook_start.setPosition(window_size.x * 0.11, window_size.y * 0.105);
		window.draw(hook_start);

		int amount_1 = 15;
		for (int i = 0; i < amount_1; i++) {
			hook_middle.setPosition(window_size.x * 0.11 + scale * (hook_start.getLocalBounds().width + i * hook_middle.getLocalBounds().width), window_size.y * 0.105);
			window.draw(hook_middle);
		}

		hook_end.setPosition(
			window_size.x * 0.11 + scale * (hook_start.getLocalBounds().width + (amount_1 - 1) * hook_middle.getLocalBounds().width - 4),
			window_size.y * 0.105);
		window.draw(hook_end);


		int amount_2 = 21;
		for (int i = 0; i < amount_2; i++) {
			left.setPosition(
				window_size.x * 0.11,
				window_size.y * 0.105 + scale * (hook_start.getLocalBounds().height + i * left.getLocalBounds().height));
			window.draw(left);
		}


		int amount_3 = 26;
		for (int i = 0; i < amount_3; i++) {
			right.setPosition(
				window_size.x * 0.82,
				window_size.y * 0.105 + scale * (hook_end.getLocalBounds().height + i * right.getLocalBounds().height));
			window.draw(right);
		}


		corner_left.setPosition(window_size.x * 0.11, window_size.y * 0.84);
		window.draw(corner_left);

		int amount_4 = 11;
		for (int i = 0; i < amount_4; i++) {
			bottom.setPosition(
				window_size.x * 0.11 + scale * (corner_left.getLocalBounds().width + i * bottom.getLocalBounds().width),
				window_size.y * 0.841);
			window.draw(bottom);
		}

		corner_right.setPosition(
			window_size.x * 0.11 + scale * (corner_left.getLocalBounds().width + (amount_4 - 1) * bottom.getLocalBounds().width + 60),
			window_size.y * 0.84);
		window.draw(corner_right);


		sf::RectangleShape fill(sf::Vector2f(window_size.x * 0.7, window_size.y * 0.71));
		fill.setFillColor(sf::Color::Color(248, 248, 243));
		fill.setPosition(sf::Vector2f(window_size.x * 0.15, window_size.y * 0.144));
		window.draw(fill);


		sf::RectangleShape under_hook(sf::Vector2f(window_size.x * 0.775, 1.7f));
		under_hook.setFillColor(sf::Color::Black);
		under_hook.setPosition(sf::Vector2f(window_size.x * 0.11, window_size.y * 0.15));
		window.draw(under_hook);

	}
	void drawLines(const float height = 1.0f, const float density = 1.0f) {
		int amount_5 = (41 * height + 1) * density;
		sf::RectangleShape horizontal_line(sf::Vector2f(window_size.x * 0.77f, 2.3f));
		horizontal_line.setFillColor(sf::Color::Color(50, 50, 50, 50));
		for (int i = 2; i < amount_5; i++) {
			horizontal_line.setPosition(sf::Vector2f(
				window_size.x * 0.115, 
				window_size.y * 0.15 + i * size / density - size * 0.65f));
			window.draw(horizontal_line);
		}


		int amount_6 = 30 * density;
		sf::RectangleShape vertical_line(sf::Vector2f(2.3f, window_size.y * 0.735f * height));
		vertical_line.setFillColor(sf::Color::Color(50, 50, 50, 50));
		for (int i = 1; i < amount_6; i++) {
			vertical_line.setPosition(sf::Vector2f(
				window_size.x * 0.115 + i * size / density - size* 0.528f,
				window_size.y * 0.15));
			window.draw(vertical_line);
		}
	}
};

Background background;