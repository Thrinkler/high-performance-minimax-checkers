#ifndef MINIMAX_H
#define MINIMAX_H
#include <cstdint>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>

#include "../../board/gameMaster.h"


using namespace std;

class Minimax {
    private:
        unordered_map<BoardState, int> umap;
        int maxDepth;
        pair<int,Move> minimax(GameMaster& gm, int depth, int alpha = -INFINITY, int beta = INFINITY, bool player=false);
        

    public:
        int evaluateBoard(GameMaster& gm, int turn, int depth = 0);
        int evaluateBoard(GameMaster& gm, bool turn, int depth = 0);
        int betterEval(GameMaster& gm, int turn);
        Minimax(int depth);
        pair<int,Move> minimax(GameMaster& gm, bool player);
};


#endif