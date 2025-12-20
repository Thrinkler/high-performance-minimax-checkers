#ifndef GAMEMASTER_H
#define GAMEMASTER_H
#include <cstdint>
#include <string>
#include <vector>
#include "board.h"

typedef uint64_t Bitboard;
using namespace std;

struct Move{
    Bitboard pos;
    bool rl, ud;
    Move(int _r, int _c, bool _rl, bool _ud) 
        : rl(_rl), ud(_ud){pos = 1ULL<<(_c+ 8*_r);}
    Move(Bitboard _pos, bool _rl, bool _ud) 
        : pos(_pos),rl(_rl), ud(_ud){}
    
};

class GameMaster{
    private:
        Board* board;
        bool player;
        bool iOwnBoard;

        stack<Move> moves;

        pair<Bitboard,Bitboard> definePlayerEnemy();

    public:
        GameMaster();
        GameMaster(Bitboard whitePieces, Bitboard redPieces, Bitboard queenPieces, bool playerToPlay = true);
        GameMaster(BoardState board, bool playerToPlay = true);
        GameMaster(Board* board, bool playerToPlay = true);
        ~GameMaster();

        bool getPlayerPlaying();

        Bitboard getWhiteMoves();
        Bitboard getRedMoves();

        void addLastMove(int r, int c, bool rl, bool ud);

        bool undoMove();

        vector<Move> getPossibleMoves();
        bool movePiece(int r, int c, bool rl, bool ud);

        stack<Move> getMoves();
        string print(Bitboard board);
        
};

#endif 