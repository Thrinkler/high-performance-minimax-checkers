#ifndef BOARD_H
#define BOARD_H
#include <string>
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

typedef uint64_t Bitboard;
struct BoardState {
    Bitboard white;
    Bitboard red;
    Bitboard queens;
    bool operator==(const BoardState& other) const {
        return (red == other.red) && 
               (white == other.white) && 
               (queens == other.queens);
    }
};
namespace std {
    template <>
    struct hash<BoardState> {
        size_t operator()(const BoardState& b) const {
            hash<unsigned long long> hasher;
            
            size_t h1 = hasher(b.red);
            size_t h2 = hasher(b.white);
            size_t h3 = hasher(b.queens);
            
            size_t final_hash = h1;
            final_hash ^= h2 + 0x9e3779b9 + (final_hash << 6) + (final_hash >> 2);
            final_hash ^= h3 + 0x9e3779b9 + (final_hash << 6) + (final_hash >> 2);

            return final_hash;
        }
    };
}
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

        int getWhiteNumPieces();
        int getRedNumPieces();
        int getWhiteKingNumPieces();
        int getRedKingNumPieces();

        BoardState getAllBoards();
        void setAllBoards(BoardState boards, int whiteNum, int redNum, int wKnum,int rKNum);
        

};

#endif 
