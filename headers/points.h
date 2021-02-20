class Points {
private:
	int points = 0;
	int lines = 0;
	int level = 1;

	int start_level = 0;
public:
	int getPoints() { return points; }
	int getLines() { return lines; }
	int getLevel() { return level; }

	void addPoints(int points) { this->points += points; }
	void addLines(int lines) { this->lines += lines; }

	void setPoints(int points) { this->points = points;	}
	void setLines(int lines) { this->lines = lines; }
	void setLevel(int level) { this->level = level; }

	void resetStats() {
		points = 0;
		lines = 0;
		level = 1;
		fall.speed = fall.init_speed;
	}
	void setStartLevel() { 
		start_level = level;
		int temp_points = points;
		for (int i = 0; i < level_amount; i++)
			if (temp_points >= 10000) {
				start_level -= 1;
				temp_points -= 10000;
			}
			else break;
	}
	void whenNextLevel() {
		if (((float)points / (level + 1 - start_level)) >= 10000) {
			level += 1;
			fall.increaseSpeed();
		}
	}
	void drawStats() {
		std::string strings[] = {"Points:", "Lines:", "Level:"};
		sf::Text text;
		sf::Vector2f pos = { render_positions.points.x + field_offset.x + size * 0.1f, render_positions.points.y + field_offset.y };

		text.setFont(font);
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::Black);
		for (int i = 0; i < 3; i++) {
			text.setString(strings[i]);
			text.setPosition(pos.x, pos.y - size * 1.9f + i * size *3);
			window.draw(text);
		}

		text.setFillColor(sf::Color::Color(0, 30, 0));
		text.setCharacterSize(10);
		std::string points_s = std::to_string(points);
		text.setString(points_s);
		text.setPosition(pos.x, pos.y - size * 0.7f);
		window.draw(text);

		std::string lines_s = std::to_string(lines);
		text.setString(lines_s);
		text.setPosition(pos.x, pos.y - size * 0.7f + size * 3);
		window.draw(text);

		std::string level_s = std::to_string(level);
		text.setString(level_s);
		text.setPosition(pos.x, pos.y - size * 0.7f + 2 * size * 3);
		window.draw(text);
	}
};

Points points;
