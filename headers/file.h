class File {
	FILE* fp;

public:
	File() {
		//manualInsert();

		fp = fopen("miscellaneous/data.dat", "rb");
		if (fp == NULL) {
			createFileTemplate();
			return;
		}

		bool show_continue;
		fread(&show_continue, sizeof(bool), 1, fp);
		mainMenu.setIfContinue(show_continue);

		int levels_unlocked;
		fread(&levels_unlocked, sizeof(int), 1, fp);
		levelsMenu.unlockLevels(levels_unlocked);

		for (int i = 0; i < score_amount; i++) {
			fread(&highscoresMenu.highscores[i], sizeof(HighscoresMenu::Highscores), 1, fp);
		}


		int temp;
		fread(&temp, sizeof(int), 1, fp);
		points.setPoints(temp);

		fread(&temp, sizeof(int), 1, fp);
		points.setLines(temp);

		fread(&temp, sizeof(int), 1, fp);
		points.setLevel(temp);

		for (int i = 0; i < field_size.area; i++) {
			fread(&previous[i], sizeof(Previous), 1, fp);
		}

		fclose(fp);

		for (int i = 0; i < field_size.area; i++) {
			if (previous[i].x != -1)
				brick_amount++;
		}

		//drukuj();
	}
	void createFileTemplate() {
		fp = fopen("miscellaneous/data.dat", "w+b");

		bool show_continue = false;
		fwrite(&show_continue, sizeof(bool), 1, fp);

		int levels_unlocked = 0;
		fwrite(&levels_unlocked, sizeof(int), 1, fp);

		HighscoresMenu::Highscores highscore{-1, " "};
		for (int i = 0; i < score_amount; i++)
			fwrite(&highscore, sizeof(highscore), 1, fp);


		int points = 0;
		fwrite(&points, sizeof(int), 1, fp);

		int lines = 0;
		fwrite(&lines, sizeof(int), 1, fp);

		int level = 0;
		fwrite(&level, sizeof(int), 1, fp);

		Previous temp = {-1, -1, -1};
		for (int i = 0; i < field_size.area; i++) {
			fwrite(&temp, sizeof(Previous), 1, fp);
		}

		fclose(fp);
	}
	void drukuj() {
		//std::cout << points.getPoints();
		/*for (int i = 0; i < field_size.area; i++) {
			std::cout << previous[i].x << " " << previous[i].y << " " << previous[i].colorId << std::endl;
		}*/
		/*if (previous[0].colorId == -1) {
			std::cout << "true: " << previous[0].colorId << std::endl;
		}*/
		/*
		std::cout << mainMenu.getLevelsUnlocked() << std::endl;
		for (int i = 0; i < score_amount; i++) {
			std::cout << highscoresMenu.highscores[i].name << std::endl;
			std::cout << highscoresMenu.highscores[i].score << std::endl;
			std::cout << std::endl;
		}*/
	}
	void savePausedGame() {
		fp = fopen("miscellaneous/data.dat", "r+b");

		bool show_continue = true;
		fwrite(&show_continue, sizeof(bool), 1, fp);

		int levels_unlocked = levelsMenu.getLevelsUnlocked();
		fwrite(&levels_unlocked, sizeof(int), 1, fp);

		fseek(fp, sizeof(highscoresMenu.highscores), SEEK_CUR);

		int temp = points.getPoints();
		fwrite(&temp, sizeof(int), 1, fp);

		temp = points.getLines();
		fwrite(&temp, sizeof(int), 1, fp);

		temp = points.getLevel();
		fwrite(&temp, sizeof(int), 1, fp);

		for (int i = 0; i < field_size.area; i++) {
			fwrite(&previous[i], sizeof(previous[i]), 1, fp);
		}
		

		fclose(fp);
	}
	void saveFinishedGame() {
		fp = fopen("miscellaneous/data.dat", "r+b");

		bool show_continue = false;
		fwrite(&show_continue, sizeof(bool), 1, fp);

		int levels_unlocked = levelsMenu.getLevelsUnlocked();
		fwrite(&levels_unlocked, sizeof(int), 1, fp);

		for (int i = 0; i < score_amount; i++) {
			fwrite(&highscoresMenu.highscores[i], sizeof(HighscoresMenu), 1, fp);
		}

		int points = 0;
		fwrite(&points, sizeof(int), 1, fp);

		int lines = 0;
		fwrite(&lines, sizeof(int), 1, fp);

		int level = 0;
		fwrite(&level, sizeof(int), 1, fp);

		Previous temp = { -1, -1, -1 };
		for (int i = 0; i < field_size.area; i++) {
			fwrite(&temp, sizeof(Previous), 1, fp);
		}

		fclose(fp);
	}
	void manualInsert() {
		fp = fopen("miscellaneous/data.dat", "w+b");
		
		bool show_continue = true;
		fwrite(&show_continue, sizeof(bool), 1, fp);

		int levels_unlocked = 0;
		fwrite(&levels_unlocked, sizeof(int), 1, fp);

		char name[score_name_size + 1] = "nazwiskoi1";
		HighscoresMenu::Highscores highscores[score_amount];
		for (int i = 0; i < score_amount; i++) {
			for (int j = 0; j < sizeof(name); j++)
				highscores[i].name[j] = name[j];
			highscores[i].score = i*1000;
		}

		for (int i = 0; i < score_amount; i++) {
			fwrite(&highscores[i], sizeof(highscores[i]), 1, fp);		
		}


		int points = 9900;
		fwrite(&points, sizeof(int), 1, fp);

		int lines = 0;
		fwrite(&lines, sizeof(int), 1, fp);

		int level = 0;
		fwrite(&level, sizeof(int), 1, fp);

		Previous temp = { -1, -1 , -1};
		for (int i = 0; i < field_size.area; i++) {
			fwrite(&temp, sizeof(Previous), 1, fp);
		}

		fclose(fp);
	}
}; File file;

