#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4244 ) 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>

#include "headers/values.h"
#include "headers/points.h"
#include "headers/blockLook.h"
#include "headers/brickRender.h"
#include "headers/BlockRender.h"
#include "headers/title.h"
#include "headers/menus.h"
#include "headers/background.h"
#include "headers/file.h"
#include "headers/sounds.h"
#include "headers/miscFunctions.h"


int WinMain()
{
    sf::Image icon;
    icon.loadFromFile("miscellaneous/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    srand(time(NULL));

    sf::View view;
    sf::Event event;

    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(fps);
    texture.loadFromFile("miscellaneous/Blocks.png");
    font.loadFromFile("miscellaneous/The Artisan Marker.otf");

    view.setCenter(sf::Vector2f(window_size.x / 2, window_size.y / 2));
    view.setSize(sf::Vector2f(window_size.x, window_size.y));
    view.zoom(0.8f);
    window.setView(view);
    menu.openMenu("mainMenu");
    float prev_window_height = window.getSize().y;

    while (window.isOpen())
    {
        if (!started)   // ouside game
        {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::Resized)
                    resize_window(prev_window_height, view);
                if (event.type == sf::Event::KeyPressed) 
                {
                    if (menu.ifInMenu("mainMenu")) {
                        switch (event.key.code) {
                        case sf::Keyboard::Up:
                            mainMenu.moveUp();
                            break;
                        case sf::Keyboard::Down:
                            mainMenu.moveDown();
                            break;
                        case sf::Keyboard::Enter:
                            switch (mainMenu.getPressedItem()) {
                            case 0:
                                continuePreviousGame();
                                break;
                            case 1:
                                menu.openMenu("levelsMenu");
                                break;
                            case 2:
                                menu.openMenu("highscoresMenu");
                                break;
                            case 3:
                                menu.openMenu("keybindingsMenu");   // keybindings
                                break;
                            case 4:
                                window.close();
                            }
                        }
                    }
                    else if (menu.ifInMenu("keybindingsMenu")) {
                        switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            menu.openMenu("mainMenu");
                            break;
                        }
                    }
                    else if (menu.ifInMenu("levelsMenu")) {
                        switch (event.key.code) {
                        case::sf::Keyboard::Escape:
                            menu.openMenu("mainMenu");
                            break;
                        case sf::Keyboard::Right:
                            levelsMenu.moveRight();
                            break;
                        case sf::Keyboard::Left:
                            levelsMenu.moveLeft();
                            break;
                        case sf::Keyboard::Enter:
                            int level = levelsMenu.getPressedItem();
                            startNewGame(level);
                            break;
                        }
                    }
                    else if (menu.ifInMenu("highscoresMenu") && !highscoresMenu.getInsertMode() && !highscoresMenu.getShowPoints()) {
                        switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            menu.openMenu("mainMenu");
                            break;
                        }
                    }
                    else if (menu.ifInMenu("highscoresMenu") && highscoresMenu.getInsertMode()) {
                        switch (event.key.code) {
                        case sf::Keyboard::Enter:
                            if (highscoresMenu.ifLongEnough()) {
                                highscoresMenu.setInsertMode(false);
                                menu.openMenu("mainMenu");
                                file.saveFinishedGame();
                            }
                            break;
                        case sf::Keyboard::BackSpace:
                            highscoresMenu.removeLetter();
                            break;
                        }
                        char letter = fromKtoS(event.key.code);
                        if (letter != '?')
                            highscoresMenu.addLetter(letter);
                    }
                    else if (menu.ifInMenu("highscoresMenu") && highscoresMenu.getShowPoints()) {
                        switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            highscoresMenu.setShowPoints(false);
                            menu.openMenu("mainMenu");
                            break;
                        }
                    }
                }
            }
            window.clear(sf::Color::Color(195, 195, 195));
            background.drawEmpty();
            menu.draw();
            window.display();
        }

        if (started && !paused)    // in-game
        {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::Resized) {
                    resize_window(prev_window_height, view);
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    switch (event.mouseButton.button) {
                    case sf::Mouse::Button::Left:
                        speedUp();
                        break;
                    case sf::Mouse::Button::Right:
                        hold();
                        break;
                    }
                }
                
                if (event.type == sf::Event::MouseButtonReleased) {
                    switch (event.mouseButton.button) {
                    case sf::Mouse::Button::Left:
                        speedDown();
                        break;
                    }
                }

                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                    case sf::Keyboard::A:
                        rotate(-1);
                        break;
                    case sf::Keyboard::D:
                        rotate(1);
                        break;
                    case sf::Keyboard::Space:
                        if (currentRender.whenDrop())
                            nextBlock();
                        break;
                    case sf::Keyboard::F1:
                        pauseGame("pauseMenu");
                        break;
                    case sf::Keyboard::Escape:
                        pauseGame("quitMenu");
                        break;
                    case sf::Keyboard::F10:
                        sounds.nextSong();
                    }
                }
            }
            if (!initialized)
                initialize();
            window.clear(sf::Color::Color(195, 195, 195));
            background.drawEmpty();
            background.drawLines();

            drawPreviousBricks();
            alignOtherRenders();
            drawOtherRenders();

            if (!currentRender.nextFrame())
                nextBlock();
            else {
                currentRender.followMouse();
                currentRender.draw();
                currentRender.drawPrediction();
                drawText();
                points.drawStats();
                drawBorders();

                window.display();
            }
        }

        if (started && paused)    // in-game, paused
        {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::Resized)
                    resize_window(prev_window_height, view);
                if (event.type == sf::Event::KeyPressed) {
                    if (menu.ifInMenu("pauseMenu")) {
                        switch (event.key.code) {
                        case sf::Keyboard::F1:
                            unpauseGame();
                            break;
                        }
                    }
                    if (menu.ifInMenu("quitMenu")) {
                        switch (event.key.code) {
                        case sf::Keyboard::Left:
                            quitMenu.moveLeft();
                            break;
                        case sf::Keyboard::Right:
                            quitMenu.moveRight();
                            break;
                        case sf::Keyboard::Enter:
                            if (quitMenu.getPressedItem() == 0)
                                quitLevel();
                            else
                                unpauseGame();
                            break;
                        case sf::Keyboard::Escape:
                            unpauseGame();
                            break;
                        }
                    }
                }
            }
            window.clear(sf::Color::Color(195, 195, 195));

            background.drawEmpty();
            background.drawLines();

            drawPreviousBricks();
            alignOtherRenders();
            drawOtherRenders();

            currentRender.draw();
            currentRender.drawPrediction();
            drawText();
            points.drawStats();
            drawBorders();

            menu.draw();

            window.display();
        }
    }
    return 0;
}