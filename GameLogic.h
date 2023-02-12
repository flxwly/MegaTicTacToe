#ifndef MEGATICTACTOE_GAMELOGIC_H
#define MEGATICTACTOE_GAMELOGIC_H

#include <array>


enum Players {
    neutral = '#', blue = 'b', red = 'r'
};

typedef std::array<std::array<Players, 3>, 3> SimpleGrid;
typedef std::array<std::array<std::array<std::array<Players, 3>, 3>, 3>, 3> LargeGrid;

class GameLogic {
public:
    GameLogic() { init(); };

    void init();

    void update(int outerX, int outerY, int innerX, int innerY);


    Players winner = Players::neutral;
    Players curPlayer = neutral;
    SimpleGrid largeState{}; // default all 0
    LargeGrid smallState{}; // default all 0

    int nextLargeMoveX = -1, nextLargeMoveY = -1;

    static Players checkLargeState(const SimpleGrid &largeState, const LargeGrid &smallState,
                                   int outerX, int outerY, int innerX, int innerY);

    static Players checkWin(const SimpleGrid &largeState, int outerX, int outerY);

    static Players checkBox(SimpleGrid box, int x, int y);

    static bool canMakeAMove(SimpleGrid box);

};


#endif //MEGATICTACTOE_GAMELOGIC_H
