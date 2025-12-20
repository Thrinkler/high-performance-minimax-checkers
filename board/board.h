#ifndef BOARD_H
#define BOARD_H
#include <cstdint>
#include <string>

typedef uint64_t Bitboard;
struct BoardState {
    Bitboard white;
    Bitboard red;
    Bitboard queens;
};

enum RLUD {
  RIGHT = true,
  LEFT = false,
  UP = false,
  DOWN = true
};

using namespace std;

class Board{
    private:
        Bitboard whitePieces;
        Bitboard redPieces;
        Bitboard queenPieces;

        int whitePieceNum;
        int redPieceNum;

        int whiteKingNum;
        int redKingNum;

    public:
        Board();
        Board(Bitboard whitePieces, Bitboard redPieces, Bitboard queenPieces);
        
        bool locatePiece(int r, int c);
        bool locatePiece(Bitboard pos);
        int movePiece(int r, int c,bool rl);
        int movePiece(Bitboard pos,bool rl);

        int movePiece(int r, int c, bool rl, bool ud);
        int movePiece(Bitboard pos, bool rl, bool ud);

        pair<Bitboard*,Bitboard*> definePlayerEnemy(int r, int c);
        pair<Bitboard*,Bitboard*> definePlayerEnemy(Bitboard pos);


        bool canJump(int r, int c, bool rl,bool ud);
        bool canJump(Bitboard move,bool rl,bool ud);
        int jump(int r, int c, bool rl, bool ud);
        int jump(Bitboard pos, bool rl, bool ud);

        bool canMove(Bitboard move,bool rl, bool ud);

        string print();
        string print(Bitboard bitboard);

        Bitboard getWhitePieces();
        Bitboard getRedPieces();
        Bitboard getQueenPieces();

        BoardState getAllBoards();
        

};

#endif 