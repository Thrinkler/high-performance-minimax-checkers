#ifndef MINIMAX_H
#define MINIMAX_H
#include <cstdint>
#include <cmath>
#include <unordered_map>
#include <random>

#ifdef _MSC_VER
    #include <intrin.h>
    #define __builtin_popcountll __popcnt64
    inline int __builtin_ctzll(unsigned long long mask) {
        unsigned long index;
        if (_BitScanForward64(&index, mask)) {
            return index;
        }
        return 64;
    }
#endif

#include "../../board/gameMaster.h"


using namespace std;

class Minimax {
    private:
    int numIt;
        unordered_map<BoardState, int> repetitionTable;
        int maxDepth;
        pair<int,Move> minimax(GameMaster& gm, int depth, int alpha = -INFINITY, int beta = INFINITY, bool player=false);
        pair<int,Move> quiescence(GameMaster& gm, int alpha = -INFINITY, int beta = INFINITY, bool player=false);

        int heatmapValuation(GameMaster& gm, int turn);

    public:
        int evaluateBoard(GameMaster& gm, int turn, int depth = 0);
        int evaluateBoard(bool oldEv, GameMaster& gm, int turn, int depth = 0);
        int evaluateBoard(GameMaster& gm, bool turn, int depth = 0);
        int betterEval(GameMaster& gm, int turn);
        Minimax(int depth);
        pair<int,Move> minimax(GameMaster& gm, bool player);
};


#endif
