#ifndef BFS_SOLVER_H
#define BFS_SOLVER_H
#include <cstdint>
#include <string>

typedef uint64_t Bitboard;
using namespace std;

class Board{
    private:
        Bitboard whitePieces;
        Bitboard redPieces;
        Bitboard queenPieces;

    public:
        Board();
        Board(Bitboard whitePieces, Bitboard redPieces, Bitboard queenPieces);
        
        bool locatePiece(int r, int c);
        bool movePiece(int r, int c,bool rl);

        bool movePiece(int r, int c, bool rl, bool ud);

        pair<Bitboard*,Bitboard*> definePlayerEnemy(int r, int c);

        bool canJump(int r, int c, bool rl);
        bool jump(int r, int c, bool rl);
        bool jump(int r, int c, bool rl, bool ud);

        string print();

};

#endif 