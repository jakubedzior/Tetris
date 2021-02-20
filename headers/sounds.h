class Sounds {
	sf::Music music;
	sf::String titles[2] = { "Tetris.wav" , "Tetris-cover.wav" };
	int index = 0;
public:
	Sounds() {
		music.openFromFile("sounds/" + titles[0]);

		music.setLoop(true);
		music.setVolume(30);
	}
	void nextSong() {
		stop();

		index++;
		if (index == 2)
			index = 0;
		music.openFromFile("sounds/" + titles[index]);

		play();
	}
	void play() {
		music.play();
	}
	void pause() {
		music.pause();
	}
	void stop() {
		music.stop();
	}
}; Sounds sounds;
