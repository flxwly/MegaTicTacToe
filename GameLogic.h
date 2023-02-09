#ifndef MEGATICTACTOE_GAMELOGIC_H
#define MEGATICTACTOE_GAMELOGIC_H

#include <array>

enum Players {
    neutral = 0, blue = 1, red = 2
};

class GameLogic {
public:
    GameLogic() { init(); };

    void init();
    void update(int outerX, int outerY, int innerX, int innerY);


    Players winner = Players::neutral;
    Players curPlayer = neutral;
    std::array<std::array<Players, 3>, 3> largeState{}; // default all 0
    std::array<std::array<std::array<std::array<Players, 3>, 3>, 3>, 3> smallState{}; // default all 0

    int nextLargeMoveX = -1, nextLargeMoveY = -1;

    Players checkLargeState(int outerX, int outerY, int innerX, int innerY);
    Players checkWin(int outerX, int outerY);
    static Players checkBox(std::array<std::array<Players, 3>, 3> box, int x, int y);
    bool canMakeAMove(std::array<std::array<Players, 3>, 3> box);

};


#endif //MEGATICTACTOE_GAMELOGIC_H
