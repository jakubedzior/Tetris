void endRound() {
    started = false;
    sounds.stop();
    levelsMenu.unlockLevels(points.getLevel());

    if (highscoresMenu.whenNew(points.getPoints()))
        highscoresMenu.setInsertMode(true);
    else {
        highscoresMenu.setShowPoints(true);
        file.saveFinishedGame();
    }
        
    menu.openMenu("highscoresMenu");
}
void nextBlock(bool store_shape = true) {   
    if (store_shape)
        currentRender.storeShape();

    currentRender = nextRender[0];
    nextRender[0] = nextRender[1];
    nextRender[1] = nextRender[2];

    currentRender.moveTo(render_positions.current);
    for (int i = 0; i < 3; i++) {
        nextRender[i].moveTo(render_positions.next[i]);
    }

    nextRender[2].init();
    currentLook[nextRender[2].shapeId].randomize();  // change .randomize() to .toDefault() to stop randomly orienting next shapes

    currentRender.whenRowFull();

    if (currentRender.ifCollides()) {
        endRound();
    }
}
void hold() {
    currentRender.just_dropped = false;
    if (!holdInitialized) {
        holdRender = currentRender;
        sf::Vector2f pos = holdRender.getObjPos();
        holdRender.moveTo(render_positions.hold);
        nextBlock(false);
        currentRender.moveTo(pos);

        holdInitialized = true;
    } else {
        sf::Vector2f currObjPos = currentRender.getObjPos();
        BlockRender tempRender = holdRender;

        holdRender = currentRender;
        currentRender = tempRender;

        currentRender.moveTo(currObjPos);
        holdRender.moveTo(render_positions.hold);
    }
    if (currentRender.ifCollides()) {
        hold();
    }
}
void drawPreviousBricks() {
    for (int i = 0; i < brick_amount; i++) {
        bricks[previous[i].colorId].draw(i);
    }
}
void drawOtherRenders() {
    for (int i = 0; i < 3; i++) {
        nextRender[i].draw();
    }

    if (holdInitialized) {
        holdRender.draw();
    }
}
void rotate(int side) {
    currentLook[currentRender.shapeId].rotate(side);
    currentRender.refreshShape();

    if (currentRender.ifCollides("all", false)) {
        currentLook[currentRender.shapeId].rotate(-side);
        currentRender.refreshShape();
        return;
    }
    if (!currentRender.ifInBoundaries()) {
        if (sf::Mouse::getPosition(window).x < window_size.x/2) {
            currentRender.move("right", false);

            if (currentRender.ifCollides("all", false)) {
                currentRender.move("left", false);
                currentLook[currentRender.shapeId].rotate(-side);
                currentRender.refreshShape();
                return;
            }
            if (!currentRender.ifInBoundaries()) {
                currentRender.move("right", false);
                if (currentRender.ifCollides()) {
                    currentRender.move("left", false);
                    currentRender.move("left", false);
                    currentLook[currentRender.shapeId].rotate(-side);
                    currentRender.refreshShape();
                    return;
                }
            }
        }
        else {
            currentRender.move("left", false);
            if (currentRender.ifCollides("all", false)) {
                currentRender.move("right", false);
                currentLook[currentRender.shapeId].rotate(-side);
                currentRender.refreshShape();
                return;
            }
            if (!currentRender.ifInBoundaries()) {
                currentRender.move("left", false);
                if (currentRender.ifCollides()) {
                    currentRender.move("right", false);
                    currentRender.move("right", false);
                    currentLook[currentRender.shapeId].rotate(-side);
                    currentRender.refreshShape();
                    return;
                }
            }
        }
    }
}
void drawTitleInGame() {
    if (!title.initialized) {
        title.tryScrambling();
        title.initialized = true;
    }
    title.drawHorizontally(window_size.x * 0.15, window_size.y * 0.15);
}
void initialize() {
    currentRender.init();

    for (int i = 0; i < 3; i++) {
        nextRender[i].init();
    }

    initialized = true;
}
void alignOtherRenders() {
    for (int i = 0; i < 3; i++) {
        int shift_x = nextRender[i].findLeftBorder();
        int shift_y = nextRender[i].findUpperBorder();

        sf::Vector2f new_pos{ render_positions.next[i].x - shift_x, render_positions.next[i].y - shift_y };
        nextRender[i].moveTo(new_pos);
    }
    if (holdInitialized) {
        int shift_x = holdRender.findLeftBorder();
        int shift_y = holdRender.findUpperBorder();
        sf::Vector2f new_pos{ render_positions.hold.x - shift_x, render_positions.hold.y - shift_y };
        holdRender.moveTo(new_pos);
    }
}
void continuePreviousGame() {
    random = time(NULL);
    for (int i = 0; i < 7; i++)
        bricks[i].refreshTextures(i);

    initialized = false;

    fall.increaseSpeed(points.getLevel());
    fall.counter = 0;

    started = true;
    mainMenu.setIfContinue(false);
    points.setStartLevel();

    sounds.play();
}
void quitLevel() {
    mainMenu.setIfContinue(true);
    menu.openMenu("mainMenu");
    quitMenu.moveRight();
    started = false;
    paused = false;

    int level = points.getLevel();
    levelsMenu.unlockLevels(level);
    file.savePausedGame();

    sounds.pause();
}
void startNewGame(int level) {

    random = time(NULL);

    for (int i = 0; i < 7; i++)
        bricks[i].refreshTextures(i);

    for (int i = 0; i < field_size.area; i++) {
        previous[i].x = -1;
        previous[i].y = -1;
    }

    brick_amount = 0;
    holdInitialized = false;
    initialized = false;
    
    started = true;
    mainMenu.setIfContinue(false);

    points.resetStats();
    points.setLevel(level);
    points.setStartLevel();

    fall.increaseSpeed(points.getLevel());
    fall.counter = 0;

    sounds.play();
}
void drawText() {
    sf::Text hold;
    hold.setFont(font);
    hold.setCharacterSize(20);
    hold.setFillColor(sf::Color::Black);
    hold.setString("Hold:");
    hold.setPosition(sf::Vector2f(render_positions.hold.x+field_offset.x, render_positions.hold.y - size * 2.15f + field_offset.y));
    window.draw(hold);

    std::string strings[] = { "Next:", "Next+1:", "Next+2:" };
    sf::Text next;
    next.setFont(font);
    next.setFillColor(sf::Color::Black);
    next.setCharacterSize(20);
    for (int i = 0; i < 3; i++) {
        next.setString(strings[i]);
        next.setPosition(sf::Vector2f(render_positions.next[i].x + field_offset.x, render_positions.next[i].y - size * 2.15f + field_offset.y));
        window.draw(next);
    }
}
void drawBorders() {
    sf::Text border;
    border.setFillColor(sf::Color::Black);
    border.setFont(font);
    border.setCharacterSize(30);
    border.setString('.');
    
    for (int x = boundaries.x1; x <= boundaries.x2; x = x + size) {
        int y = boundaries.y2;
        border.setPosition(sf::Vector2f(x + field_offset.x + size*0.2, y + field_offset.y));
        window.draw(border);
    }
    for (int y = boundaries.y1 + size*2; y <= boundaries.y2; y = y + size) {
        int x = boundaries.x1 - size;
        border.setPosition(sf::Vector2f(x + field_offset.x + size * 0.2, y + field_offset.y));
        window.draw(border);
    }
    for (int y = boundaries.y1 + size * 2; y <= boundaries.y2; y = y + size) {
        int x = boundaries.x2;
        border.setPosition(sf::Vector2f(x + field_offset.x + size * 0.2, y + field_offset.y));
        window.draw(border);
    }
}
void pauseGame(std::string method) {
    paused = true;
    menu.openMenu(method);
    sounds.pause();
}
void unpauseGame() {
    paused = false;
    sounds.play();
}
void speedUp() {
    fall.speed /= 3.f;
}
void speedDown() {
    fall.speed *= 3.0f;
}

char fromKtoS(const sf::Keyboard::Key& k) {
    char ret;
    switch (k) {
    case sf::Keyboard::A:
        ret = 'a';
        break;
    case sf::Keyboard::B:
        ret = 'b';
        break;
    case sf::Keyboard::C:
        ret = 'c';
        break;
    case sf::Keyboard::D:
        ret = 'd';
        break;
    case sf::Keyboard::E:
        ret = 'e';
        break;
    case sf::Keyboard::F:
        ret = 'f';
        break;
    case sf::Keyboard::G:
        ret = 'g';
        break;
    case sf::Keyboard::H:
        ret = 'h';
        break;
    case sf::Keyboard::I:
        ret = 'i';
        break;
    case sf::Keyboard::J:
        ret = 'j';
        break;
    case sf::Keyboard::K:
        ret = 'k';
        break;
    case sf::Keyboard::L:
        ret = 'l';
        break;
    case sf::Keyboard::M:
        ret = 'm';
        break;
    case sf::Keyboard::N:
        ret = 'n';
        break;
    case sf::Keyboard::O:
        ret = 'o';
        break;
    case sf::Keyboard::P:
        ret = 'p';
        break;
    case sf::Keyboard::Q:
        ret = 'q';
        break;
    case sf::Keyboard::R:
        ret = 'r';
        break;
    case sf::Keyboard::S:
        ret = 's';
        break;
    case sf::Keyboard::T:
        ret = 't';
        break;
    case sf::Keyboard::U:
        ret = 'u';
        break;
    case sf::Keyboard::V:
        ret = 'v';
        break;
    case sf::Keyboard::W:
        ret = 'w';
        break;
    case sf::Keyboard::X:
        ret = 'x';
        break;
    case sf::Keyboard::Y:
        ret = 'y';
        break;
    case sf::Keyboard::Z:
        ret = 'z';
        break;
    case sf::Keyboard::Num0:
        ret = '0';
        break;
    case sf::Keyboard::Num1:
        ret = '1';
        break;
    case sf::Keyboard::Num2:
        ret = '2';
        break;
    case sf::Keyboard::Num3:
        ret = '3';
        break;
    case sf::Keyboard::Num4:
        ret = '4';
        break;
    case sf::Keyboard::Num5:
        ret = '5';
        break;
    case sf::Keyboard::Num6:
        ret = '6';
        break;
    case sf::Keyboard::Num7:
        ret = '7';
        break;
    case sf::Keyboard::Num8:
        ret = '8';
        break;
    case sf::Keyboard::Num9:
        ret = '9';
        break;
    default:
        ret = '?';
    }
    return ret;

    // Function source: https://en.sfml-dev.org/forums/index.php?topic=15226.0
}
void resize_window(float& prev_window_height, sf::View& view) {
    float scale = window.getSize().y / window_size.y;
    if (prev_window_height != window.getSize().y) {
        window.setSize(sf::Vector2u(window_size.x * scale, window.getSize().y));
        view.setSize(sf::Vector2f(window.getSize()));
    }
    else {
        scale = window.getSize().x / window_size.x;
        if (scale < 1) {
            window.setSize(sf::Vector2u(window.getSize().x, window_size.y * scale));
            view.setSize(sf::Vector2f(window.getSize()));
        }
        else {
            window.setSize(sf::Vector2u(window_size.x, window_size.y));
        }
    }
    prev_window_height = window.getSize().y;
}

