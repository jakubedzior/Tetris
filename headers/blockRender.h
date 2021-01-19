class BlockRender
{
private:
	sf::Sprite brickObject;
    int blockShape[25]; 
    sf::Vector2f obj_pos = render_positions.current;


public:
    int shapeId;


    BlockRender() {
        brickObject.setTexture(texture);
    };
    sf::Vector2f getObjPos() {
        return obj_pos;
    }
    void moveTo(sf::Vector2f value) {
        obj_pos = value;
    }
    void refreshShape() {
        int* shape = currentLook[shapeId].getShape();
        for (int i = 0; i < 25; i++) {
            blockShape[i] = *(shape + i);
        }
    }
    void init() {
        shapeId = std::rand() % 7;
        brickObject.setTextureRect(sf::IntRect(((shapeId + random) % 7) * size, 0, size, size));
        refreshShape();
    }
    int findLeftBorder() {
        int min = 5, y;
        for (int i = 0; i < 25; i++) {
            if (blockShape[i] == 1) {
                y = i / 5;
                if (y < min)
                    min = y;
            }
        }
        return min*size;
    }
    int findUpperBorder() {
        int min = 5, y;
        for (int i = 0; i < 25; i++) {
            if (blockShape[i] == 1) {
                y = i % 5;
                if (y < min)
                    min = y;
            }
        }
        return min * size;
    }
    bool nextFrame() {
        fall.counter++;
        if (fall.counter >= fall.speed) {
            if (ifCollides("bottom")) {
                return false;
            }
            obj_pos.y += size;
            fall.counter -= fall.speed;
        }
        return true;
    }
    bool move(const char side[5], bool check = true) {
        if (!ifCollides(side) || !check) {
            if (strcmp(side, "left") == 0)
                obj_pos.x -= size;
            if (strcmp(side, "right") == 0)
                obj_pos.x += size;
            return true;
        }
        return false;
    }
    void followMouse() {
        int onLeft = 0, onRight = 0;
        int onLeftArray[2] = { -1, -1 }, onRightArray[2] = { -1, -1 };
        for (int i = 0; i < 25; i++) {
            if (blockShape[i] == 1) {
                int y = i / 5;

                if (y == 0 || y == 1) {
                    bool exists = false;
                    for (int j = 0; j < 2; j++) {
                        if (y == onLeftArray[j]) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        onLeftArray[onLeft] = y;
                        onLeft++;
                    }
                }
                if (y == 3 || y == 4) {
                    bool exists = false;
                    for (int j = 0; j < 2; j++) {
                        if (y == onRightArray[j]) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        onRightArray[onRight] = y;
                        onRight++;
                    }
                }
            }
        }
     
        float center = obj_pos.x + field_offset.x + size * 2.5;
        if (onLeft > onRight) {
            center -= size / 2;
        }
        if (onLeft < onRight) {
            center += size / 2;
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (center - size / 2 > pixelPos.x) {
            if (move("left"))
                followMouse();
        }
        if (center + size / 2 < pixelPos.x) {
            if (move("right"))
                followMouse();
        }
    }
    void storeShape() {
        for (int i = 0; i < 25; i++) {
            if (blockShape[i] == 1) {
                points.addPoints(10);

                int x = size * (i / 5) + obj_pos.x;
                int y = size * (i % 5) + obj_pos.y;

                for (int j = 0; j < field_size.area; j++) {
                    if ((previous[j].x == -1) && (previous[j].y == -1)) {
                        previous[j] = { x, y };
                        previous[j].colorId = shapeId;

                        brick_amount++;
                        break;
                    }
                }
            }
        }
    }
    void draw(int offset_x = 0, int offset_y = 0, int opacity = 255) {
        for (int i = 0; i < 25; i++) {
            if (blockShape[i] == 1) {
                int x = size * (i / 5) + obj_pos.x + field_offset.x + offset_x;
                int y = size * (i % 5) + obj_pos.y + field_offset.y + offset_y;
                brickObject.setPosition(x, y);  

                brickObject.setColor(sf::Color(255, 255, 255, opacity));

                window.draw(brickObject);
            }
        }
    }
    void drawPrediction() {
        int PREDICTED_offset_y = 0;
        int ORIG_obj_pos_y = obj_pos.y;

        while (true) {
            if (!ifCollides("bottom")) {
                obj_pos.y += size;
            } else { 
                PREDICTED_offset_y = obj_pos.y - ORIG_obj_pos_y;
                break;
            }
        }
        obj_pos.y = ORIG_obj_pos_y;
        if (PREDICTED_offset_y != 0) {
            draw(0, PREDICTED_offset_y, 80);
        }
    }
    bool whenDrop() {
        while (true) {
            if (!ifCollides("bottom")) {
                obj_pos.y += size;
                draw();
            } else { break; }
        }
        if (ifCollides("left") && ifCollides("right") && ifCollides("bottom"))
            return true;
        return false;
    }
    bool ifCollides(const char side[5] = "", bool border = true) {
        int offset_x = 0;
        int offset_y = 0;

        if (strcmp(side, "left") == 0)
            offset_x = -size;
        if (strcmp(side, "right") == 0)
            offset_x = size;
        if (strcmp(side, "bottom") == 0)
            offset_y = size;
        if (strcmp(side, "2bottom") == 0)
            offset_y = 2*size;

        if (border) {
            if (!ifInBoundaries(offset_x, offset_y)) {
                return true;
            }
        }

        for (int j = 0; j < 25; j++) {
            if (blockShape[j] == 1) {
                int x = size * (j / 5) + offset_x + obj_pos.x;
                int y = size * (j % 5) + offset_y + obj_pos.y;

                for (int i = 0; i < brick_amount; i++) {
                    if (previous[i].x == x && previous[i].y == y) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void whenRowFull() {
        int combo = 0;
        for (int i = 0; i < field_size.y; i++) {
            int linePos =  (field_size.y - i) * size;
            int counter = 0;
            for (int j = 0; j < brick_amount; j++) {
                if (previous[j].y == linePos) {
                    counter++;
                }
            }
            if (counter == 10) {
                deleteLine((field_size.y - i)*size);
                i--;
                combo += 1;
                points.addLines(1);
            }
        }
        points.addPoints(10 * 10 * combo * combo);
        points.whenNextLevel();
    }
    void sortPrevious() {
        while (true) {
            bool keepOn[2] = {false, false};
            int free_index, full_index;
            for (int i = 0; i < field_size.area; i++) {
                if ((previous[i].x == -1) && (previous[i].y == -1)) {
                    free_index = i;
                    keepOn[0] = true;
                    break;
                }
            }
            for (int i = field_size.area - 1; i >= 0; i--) {
                if ((previous[i].x != -1) && (previous[i].y != -1)) {
                    full_index = i;
                    keepOn[1] = true;
                    break;
                }
            }

            if ((keepOn[0] && keepOn[1]) && (free_index < full_index)) {
                previous[free_index] = previous[full_index];
                previous[full_index] = { -1, -1 };
                continue;
            }
            return;
        }
    }
    void deleteLine(int pos_y) {
        for (int i = 0; i < brick_amount; i++) {
            if (previous[i].y == pos_y) {
                previous[i] = { -1, -1 };
            }
        }
        brick_amount -= 10;
        sortPrevious();
        for (int i = 0; i < brick_amount; i++) {
            if (previous[i].y < pos_y) {
                previous[i].y += size;
            }
        }
    }
    bool ifInBoundaries(int offset_x = 0, int offset_y = 0) {
        for (int i = 0; i < 25; i++) {
            if (blockShape[i] == 1) {
                int x = size * (i / 5) + obj_pos.x + offset_x;
                int y = size * (i % 5) + obj_pos.y + offset_y;

                if (!((boundaries.x1 <= x) && (x < boundaries.x2) && (boundaries.y1 < y) && (y <= boundaries.y2))) {
                    return false;
                }
            }
        }
        return true;
    }
};


BlockRender currentRender;
BlockRender nextRender[3];
BlockRender holdRender;
