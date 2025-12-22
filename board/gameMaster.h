#ifndef GAMEMASTER_H
#define GAMEMASTER_H
#include <cstdint>
#include <string>
#include <vector>
#include <stack>
#include "board.h"

typedef uint64_t Bitboard;
using namespace std;

struct Move{
    Bitboard pos;
    bool rl, ud;
    Bitboard capturedPos;
    bool becameQueen;
    Move(int _r, int _c, bool _rl, bool _ud, Bitboard cap = 0, bool Queen = false) 
        : rl(_rl), ud(_ud), capturedPos(cap),becameQueen(Queen){pos = 1ULL<<(_c+ 8*_r);}
    Move(Bitboard _pos, bool _rl, bool _ud, Bitboard cap = 0, bool Queen = false) 
        : pos(_pos),rl(_rl), ud(_ud), capturedPos(cap),becameQueen(Queen){}
    
    string str(){
        return "Move piece in coords: " + to_string(__builtin_ctzll(pos)/8) + " " + to_string(__builtin_ctzll(pos)%8) +
               (rl? " RIGHT ":" LEFT ") + (ud? " DOWN ":" UP ") +
               (capturedPos != 0 ? (" Capture piece " + to_string(__builtin_ctzll(capturedPos)/8) + "," + to_string(__builtin_ctzll(capturedPos)%8)) : "") +
               (becameQueen ? " Become Queen": "");
    }
};

struct Snapshot{
    BoardState boards;
    Bitboard multijumpPiecePos;
    int whiteNum;
    int redNum;
    int whiteKNum;
    int redKNum;
    bool changedTurns;
    Snapshot(BoardState b, int wn, int rn, int wkn, int rkn, bool ct = false, Bitboard mjpp = 0)
        : boards(b), whiteNum(wn), redNum(rn), whiteKNum(wkn), redKNum(rkn), changedTurns(ct), multijumpPiecePos(mjpp){}
};

class GameMaster{
    private:
        
        bool player;
        bool iOwnBoard;

        Bitboard multiJumpPiecePos = 0;

        stack<Move> moves;
        stack<Snapshot> prevBoard;

        pair<Bitboard,Bitboard> definePlayerEnemy();

    public:
        Board* board;
        
        GameMaster();
        GameMaster(Bitboard whitePieces, Bitboard redPieces, Bitboard queenPieces, bool playerToPlay = true);
        GameMaster(BoardState board, bool playerToPlay = true);
        GameMaster(Board* board, bool playerToPlay = true);
        ~GameMaster();

        bool getPlayerPlaying();

        Bitboard getWhiteMoves();
        Bitboard getRedMoves();

        bool undoMove();

        int whoWon();

        vector<Move> getPossibleMoves();
        bool movePiece(int r, int c, bool rl, bool ud);
        bool movePiece(Bitboard pos, bool rl, bool ud);

        stack<Move> getMoves();
        Board& getBoard();
        string print(Bitboard board);
        
};

#endif 