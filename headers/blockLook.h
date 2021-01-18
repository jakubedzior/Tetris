class BlockLook {
private:
    int(*shape)[5];

public:
    int rotations = 0;

    BlockLook(int pattern[5][5]) {
        shape = pattern;
 
        rotate(1);
        rotations = 0;
        randomize();    // comment this line if you want to start game with unoriented blocks
    }
    void toDefault() {
        rotate(4 - (rotations % 4));
        rotations = 0;
    }
    void randomize() {
        rotate(std::rand() % 4);
    }
    int* getShape() {
        return *shape;
    }
    void printShape() {
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++)
                std::cout << *(shape[row] + col) << ' ';
            std::cout << std::endl;
        }
    }
    void rotate(int times) {
        if (times < 0) {
            times = ((-times) % 4 + 2) % 4;
        } else {
            times = times % 4;
        }
        for (int time = 0; time < times; time++)
        {
            int newShape[5][5];
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++)
                    newShape[4 - row][col] = *(shape[col] + row);
            }
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++)
                    *(shape[row] + col) = newShape[row][col];
            }
            rotations++;
        }
    }
};

BlockLook currentLook[7] {
    {new int[5][5]{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} }}, // hero
    {new int[5][5]{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0} }}, // blue ricky
    {new int[5][5]{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 1, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0} }}, // orange ricky
    {new int[5][5]{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 0, 0, 0} }}, // smashboy
    {new int[5][5]{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 1, 0}, {0, 1, 1, 0, 0}, {0, 0, 0, 0, 0} }}, // rhode island z
    {new int[5][5]{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0} }}, // teewee
    {new int[5][5]{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 1, 0}, {0, 0, 0, 0, 0} }}  // cleeveland z
};

