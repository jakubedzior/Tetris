class MainMenu {
private:
	const int option_amount = 5;
	sf::Text options[5];
	sf::Vector2f positions[5];

	int selected_index;
	bool show_continue = false;
public:
	MainMenu() {
		for (int i = 0; i < option_amount; i++)
			positions[i] = (sf::Vector2f(window_size.x * 0.15, window_size.y * (0.5 + 0.4 / (option_amount + 1) * (1 + i))));

		std::string strings[] = { "Continue", "New game", "High scores", "Help", "Exit" };
		for (int i = 0; i < option_amount; i++) {
			options[i].setFont(font);
			options[i].setString(strings[i]);
			options[i].setCharacterSize(30);
			options[i].setOrigin(options[i].getLocalBounds().width / 2.0f,
				options[i].getLocalBounds().height / 2.0f);
		}
	}
	void init() {
		selected_index = !show_continue;
		
		for (int i = 0; i < option_amount - !show_continue; i++) {
			options[i + !show_continue].setPosition(positions[i + !show_continue]);
			options[i + !show_continue].setFillColor(sf::Color::Black);
		}
		options[!show_continue].setFillColor(sf::Color::Red);
	}
	void setIfContinue(bool show_continue) {
		this->show_continue = show_continue;
		init();
	}
	bool getIfContinue() { return show_continue; }
	void moveUp() {
		if (selected_index - 1 >= !show_continue) {
			options[selected_index].setFillColor(sf::Color::Black);
			selected_index--;
			options[selected_index].setFillColor(sf::Color::Red);

			title.initialized = false;
		}
		else {
			for (int i = 0; i < option_amount - 1 - !show_continue; i++)
				moveDown();
		}
	}
	void moveDown() {
		if (selected_index + 1 < option_amount) {
			options[selected_index].setFillColor(sf::Color::Black);
			selected_index++;
			options[selected_index].setFillColor(sf::Color::Red);

			title.initialized = false;
		}
		else {
			for (int i = 0; i < option_amount - 1 - !show_continue; i++)
				moveUp();
		}
	}
	int getPressedItem() { return selected_index; }
	void draw() {
		title.tryScrambling();
		title.drawObliquely(window_size.x * 0.5 - 10 * size, window_size.y * 0.18f, 1.3);
		
		for (int i = !show_continue; i < option_amount; i++) {
			window.draw(options[i]);
		}
	}
}; MainMenu mainMenu;

class HighscoresMenu {
	bool insert_mode = false;
	bool show_points = false;
	int full_rows = 0;
public:
	int index;
	struct Highscores {
		int score;
		char name[score_name_size + 1];
	};
	Highscores highscores[score_amount];

	HighscoresMenu() {
		for (int i = 0; i < score_amount; i++) {
			highscores[i].score = -1;
		}
	}
	void init() {
		full_rows = 0;
		for (int i = 0; i < score_amount; i++) {
			if (highscores[i].score != -1)
				full_rows++;
		}
	}
	void draw() {
		sf::Text text;
		text.setFont(font);
		if (!insert_mode && !show_points) {
			title.tryScrambling();
			title.drawHorizontally(window_size.x * 0.5 - 10.25 * size, window_size.y * 0.2);
		}
		else {
			text.setCharacterSize(45);
			text.setFillColor(sf::Color::Black);
			text.setString("Game Over");
			text.setOrigin(text.getLocalBounds().width / 2.0f,
				text.getLocalBounds().height / 2.0f);
			text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.23));
			window.draw(text);
		}
		if (show_points) {
			text.setCharacterSize(15);
			text.setFillColor(sf::Color::Color(0, 30, 0));
			text.setString("Your points: " + std::to_string(points.getPoints()));
			text.setOrigin(text.getLocalBounds().width / 2.0f,
				text.getLocalBounds().height / 2.0f);
			text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.27));
			window.draw(text);
		}

		
		float cols[] = { 0.2f, 0.35f, 0.65f };

		text.setCharacterSize(30);
		text.setString("Highscores:");
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.35));
		window.draw(text);

		std::string string;
		if (!insert_mode || show_points)
			string = "Press Escape to go back";
		else
			string = "Type in you name and press Enter.";
		text.setCharacterSize(10);
		text.setStyle(sf::Text::Style::Italic);
		text.setFillColor(sf::Color::Red);
		text.setString(string);
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.85));
		window.draw(text);

		std::string strings[] = { "Place:", "Player:", "Score:" };
		text.setCharacterSize(16);
		text.setOrigin(0, 0);
		text.setFillColor(sf::Color::Red);
		for (int i = 0; i < 3; i++) {
			text.setString(strings[i]);
			text.setPosition(sf::Vector2f(window_size.x * (cols[i]), window_size.y * 0.4));
			window.draw(text);
		}

		text.setCharacterSize(14);
		text.setStyle(sf::Text::Style::Regular);
		for (int i = 0; i < full_rows; i++) {
			text.setString(std::to_string(i+1) + ".");
			text.setPosition(sf::Vector2f(window_size.x * cols[0], window_size.y * (0.45 + 0.035 * i)));
			text.setFillColor(sf::Color::Black);
			if (i==index && insert_mode)
				text.setFillColor(sf::Color::Color(0, 30, 0));
			window.draw(text);
		}

		text.setCharacterSize(14);
		text.setOrigin(0, 0);
		for (int i = 0; i < full_rows; i++) {
			text.setString(highscores[i].name);
			text.setPosition(sf::Vector2f(window_size.x * cols[1], window_size.y * (0.45 + 0.035 * i)));
			text.setFillColor(sf::Color::Black);
			if (i == index && insert_mode)
				text.setFillColor(sf::Color::Color(0, 30, 0));
			window.draw(text);
		}

		text.setCharacterSize(14);
		for (int i = 0; i < full_rows; i++) {
			text.setString(std::to_string(highscores[i].score));
			text.setPosition(sf::Vector2f(window_size.x * cols[2], window_size.y * (0.45 + 0.035 * i)));
			text.setFillColor(sf::Color::Black);
			if (i == index && insert_mode)
				text.setFillColor(sf::Color::Color(0, 30, 0));
			window.draw(text);
		}
	}
	void setInsertMode(bool value) { insert_mode = value; }
	bool getInsertMode() { return insert_mode; }
	void setShowPoints(bool value) { show_points = value; }
	bool getShowPoints() { return show_points; }
	bool whenNew(int score) {
		for (int i = 0; i < score_amount; i++) {
			if (score > highscores[i].score) {
				index = i;
				pushDown(index);
				highscores[index].score = score;

				for (int i = 0; i < score_name_size; i++)
					highscores[index].name[i] = ' ';
				return true;
			}
		}
		return false;
	}
	void pushDown(int row) {
		for (int i = score_amount - 1; i > row; i--) {
			highscores[i] = highscores[i - 1];
		}
	}
	void addLetter(char letter) {
		for (int i = 0; i < score_name_size; i++) {
			if (highscores[index].name[i] == ' ') {
				highscores[index].name[i] = letter;
				return;
			}
		}
	}
	void removeLetter() {
		for (int i = score_name_size-1; i >= 0; i--)
			if (highscores[index].name[i] != ' ') {
				highscores[index].name[i] = ' ';
				return;
			}
	}
	bool ifLongEnough() {
		if (highscores[index].name[0] != ' ')
			return true;
		return false;
	}
}; HighscoresMenu highscoresMenu;

class KeybindingsMenu {
private:
	const int string_amount = 10;
	std::string strings[10]{
		"Mouse:",
		"- move:  change position",
		"- hold left:  temporary speed increase",
		"- right click:  hold block for later",

		"Keyboard:",
		"- A, D:  rotate",
		"- space:  drop to the bottom",
		"- F1:  pause / unpause",
		"- Escape:  save and quit",
		"- F10: change music"
	};
	float spacings[10] = { 0.f, 0.04f, 0.07f, 0.1f, 0.16f, 0.2f, 0.23f, 0.27f, 0.3f, 0.34f };

public:
	void draw() {
		title.tryScrambling();
		title.drawHorizontally(window_size.x * 0.5 - 10.25 * size, window_size.y * 0.2);

		sf::Text text;
		text.setFont(font);

		text.setCharacterSize(30);
		text.setFillColor(sf::Color::Black);
		text.setString("Keybindings:");
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.35));
		window.draw(text);

		text.setCharacterSize(10);
		text.setStyle(sf::Text::Style::Italic);
		text.setFillColor(sf::Color::Red);
		text.setString("Press Escape to go back");
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.85));
		window.draw(text);

		text.setOrigin(0, 0);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Style::Regular);
		for (int i = 0; i < string_amount; i++) {
			if (i == 0 || i == 4)
				text.setCharacterSize(17);
			else
				text.setCharacterSize(12);
			text.setString(strings[i]);
			text.setPosition(sf::Vector2f(window_size.x * 0.15, window_size.y * (0.43 + spacings[i])));
			window.draw(text);
		}
	}
}; KeybindingsMenu keybindingsMenu;

class PauseMenu {
private:
	const int string_amount = 10;
	std::string strings[10]{
		"Mouse:",
		"- move:  change position",
		"- hold left:  temporary speed increase",
		"- right click:  hold block for later",

		"Keyboard:",
		"- A, D:  rotate",
		"- space:  drop to the bottom",
		"- F1:  pause / unpause",
		"- Escape:  save and quit",
		"- F10: change music"
	};
	float spacings[10] = {0.f, 0.04f, 0.07f, 0.1f, 0.16f, 0.2f, 0.23f, 0.27f, 0.3f, 0.34f };
public:
	void draw() {
		sf::RectangleShape dim(window_size);
		dim.setFillColor(sf::Color(0, 0, 0, 230));
		window.draw(dim);

		sf::Text text;
		text.setFont(font);

		text.setCharacterSize(35);
		text.setString("Game paused");
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.2));
		window.draw(text);

		text.setCharacterSize(10);
		text.setStyle(sf::Text::Style::Italic);
		text.setFillColor(sf::Color::Red);
		text.setString("Press F1 to unpause");
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.25));
		window.draw(text);


		text.setStyle(sf::Text::Style::Regular);
		text.setCharacterSize(25);
		text.setString("Keybindings:");
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.35));
		window.draw(text);


		text.setOrigin(0, 0);
		for (int i = 0; i < string_amount; i++) {
			if (i == 0 || i == 4)
				text.setCharacterSize(17);
			else
				text.setCharacterSize(12);

			text.setString(strings[i]);
			text.setPosition(sf::Vector2f(window_size.x * 0.15, window_size.y * (0.4 + spacings[i])));
			window.draw(text);
		}
	}
}; PauseMenu pauseMenu;

class QuitMenu {
private:
	const int option_amount = 2;
	sf::Text options[2];

	int selected_index = 1;
public:
	QuitMenu() {
		std::string strings[] = { "Yes", "No" };
		sf::Color colors[2] = { sf::Color::White, sf::Color::Red };
		for (int i = 0; i < option_amount; i++) {
			options[i].setFont(font);
			options[i].setCharacterSize(20);
			options[i].setString(strings[i]);
			options[i].setFillColor(colors[i]);
			options[i].setPosition(sf::Vector2f(window_size.x * (0.3 + i * 0.4), window_size.y * 0.5));
		}
	}
	void draw() {
		sf::RectangleShape dim(window_size);
		dim.setFillColor(sf::Color(0, 0, 0, 230));
		window.draw(dim);

		sf::Text text[2];
		std::string strings[2] = { "Are you sure", "you want to leave?" };
		for (int i = 0; i < 2; i++) {
			text[i].setFont(font);
			text[i].setFillColor(sf::Color::White);
			text[i].setCharacterSize(30);
			text[i].setString(strings[i]);
			text[i].setOrigin(text[i].getLocalBounds().width / 2.0f,
				text[i].getLocalBounds().height / 2.0f);
			text[i].setPosition(sf::Vector2f(window_size.x / 2, window_size.y * (0.3 + 0.04 * i)));
			window.draw(text[i]);
		}

		for (int i = 0; i < option_amount; i++) {
			window.draw(options[i]);
		}
	}
	void moveRight() {
		if (selected_index == 0) {
			options[selected_index].setFillColor(sf::Color::White);
			selected_index++;
			options[selected_index].setFillColor(sf::Color::Red);
		}
	}
	void moveLeft() {
		if (selected_index == 1) {
			options[selected_index].setFillColor(sf::Color::White);
			selected_index--;
			options[selected_index].setFillColor(sf::Color::Red);
		}
	}
	int getPressedItem() {
		return selected_index;
	}
}; QuitMenu quitMenu;

class LevelsMenu {
private:
	sf::Text title_;
	sf::Text comment[2];

	const int option_amount = level_amount;
	sf::Text options[level_amount];
	int selected_index = 0;

	int levels_unlocked = 0;
public:
	void init() {
		selected_index = 0;

		title_.setFont(font);
		title_.setCharacterSize(30);
		title_.setString("Choose level:");
		title_.setFillColor(sf::Color::Black);
		title_.setOrigin(title_.getLocalBounds().width / 2.0f,
			title_.getLocalBounds().height / 2.0f);
		title_.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.4));

		std::string strings[2] = { "The lower level you start with,", "the easier it is to set a new high score." };
		for (int i = 0; i < 2; i++) {
			comment[i].setFont(font);
			comment[i].setCharacterSize(10);
			comment[i].setString(strings[i]);
			comment[i].setFillColor(sf::Color::Black);
			comment[i].setOrigin(comment[i].getLocalBounds().width / 2.0f,
				comment[i].getLocalBounds().height / 2.0f);
			comment[i].setPosition(sf::Vector2f(window_size.x / 2, window_size.y * (0.55 + 0.03 * i)));
		}

		for (int i = 0; i < option_amount; i++) {
			options[i].setFont(font);
			options[i].setCharacterSize(20);
			options[i].setString(std::to_string(i));
			options[i].setFillColor(sf::Color::Black);
			options[i].setPosition(sf::Vector2f(window_size.x * (0.5 - (0.06 * (option_amount/2 - i)) - 0.02), window_size.y * 0.45));
		}

		options[0].setFillColor(sf::Color::Red);
		for (int i = levels_unlocked + 1; i < option_amount; i++)
			options[i].setFillColor(sf::Color::Color(128, 128, 128));
	}
	void unlockLevels(int level) {
		if (levels_unlocked < level) {
			levels_unlocked = level;
			if (levels_unlocked > option_amount - 1)
				levels_unlocked = option_amount - 1;
		}
	}
	int getLevelsUnlocked() { return levels_unlocked; }
	void draw() {
		title.tryScrambling();
		title.drawHorizontally(window_size.x * 0.5 - 10.25 * size, window_size.y * 0.2);

		window.draw(title_);
		for (int i = 0; i < 2; i++)
			window.draw(comment[i]);
		for (int i = 0; i < option_amount; i++)
			window.draw(options[i]);

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(10);
		text.setStyle(sf::Text::Style::Italic);
		text.setFillColor(sf::Color::Red);
		text.setString("Press Escape to go back");
		text.setOrigin(text.getLocalBounds().width / 2.0f,
			text.getLocalBounds().height / 2.0f);
		text.setPosition(sf::Vector2f(window_size.x / 2, window_size.y * 0.85));
		window.draw(text);
	}
	void moveRight() {
		if (levels_unlocked == 0)
			return;
		if (selected_index < levels_unlocked) {
			options[selected_index].setFillColor(sf::Color::Black);
			selected_index++;
			options[selected_index].setFillColor(sf::Color::Red);
		}
		else {
			for (int i = 0; i < levels_unlocked; i++)
				moveLeft();
		}
	}
	void moveLeft() {
		if (levels_unlocked == 0)
			return;
		if (selected_index > 0) {
			options[selected_index].setFillColor(sf::Color::Black);
			selected_index--;
			options[selected_index].setFillColor(sf::Color::Red);
		}
		else {
			for (int i = 0; i < levels_unlocked; i++)
				moveRight();
		}
	}
	int getPressedItem() { return selected_index; }

}; LevelsMenu levelsMenu;
	

class MenuHandler {
	std::string menu;
	
public:
	void openMenu(std::string menu) {
		this->menu = menu;
		title.initialized = false;

		if (menu == "mainMenu")
			mainMenu.init();
		if (menu == "levelsMenu")
			levelsMenu.init();
		if (menu == "highscoresMenu")
			highscoresMenu.init();
	}
	bool ifInMenu(std::string menu) {
		if (this->menu == menu)
			return true;
		return false;
	}
	void draw() {
		if (menu == "mainMenu")
			mainMenu.draw();
		if (menu == "keybindingsMenu")
			keybindingsMenu.draw();
		if (menu == "pauseMenu")
			pauseMenu.draw();
		if (menu == "quitMenu")
			quitMenu.draw();
		if (menu == "levelsMenu")
			levelsMenu.draw();
		if (menu == "highscoresMenu")
			highscoresMenu.draw();
	}
}; MenuHandler menu;
