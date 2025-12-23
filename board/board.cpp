#include "board.h"
#include<iostream>
using namespace std;

using BoardPair = std::pair<Bitboard&, Bitboard&>;

const Bitboard NOT_0_FILE = 0xfefefefefefefefeULL;
const Bitboard NOT_7_FILE = 0x7f7f7f7f7f7f7f7fULL;

Board::Board(){
    whitePieces = 6172839697753047040;
    redPieces = 11163050;
    queenPieces = 0;

    whitePieceNum = 12;
    redPieceNum = 12;
    whiteKingNum = 0;
    redKingNum = 0;

}

 Board::Board(Bitboard wP, Bitboard rP, Bitboard qP){
    whitePieces = wP;
    redPieces = rP;
    queenPieces = qP;

    whitePieceNum = __builtin_popcountll(wP);
    redPieceNum   = __builtin_popcountll(rP);
    whiteKingNum  = __builtin_popcountll(wP & qP);
    redKingNum    = __builtin_popcountll(rP & qP);
 }

bool Board::locatePiece(int r, int c){
    Bitboard completeBoard = redPieces | whitePieces;

    Bitboard pos = 1ULL<<(c+ 8*r);

    return (completeBoard&pos) != 0;
}

bool Board::locatePiece(Bitboard pos){
    Bitboard completeBoard = redPieces | whitePieces;

    return (completeBoard&pos) != 0;
}

pair<Bitboard*,Bitboard*> Board::definePlayerEnemy(int r, int c){
    Bitboard pos = 1ULL<<(c+ 8*r);

    return definePlayerEnemy(pos);
}

pair<Bitboard*,Bitboard*> Board::definePlayerEnemy(Bitboard pos){
    if (redPieces & pos) return { &redPieces, &whitePieces }; 
    else if (whitePieces & pos)return { &whitePieces, &redPieces };
    return  {nullptr, nullptr };
}



int Board::jump(int r, int c,bool rl, bool ud){
    Bitboard pos = 1ULL<<(c+ 8*r);
    return jump(pos,rl,ud);
}

int Board::jump(Bitboard pos,bool rl, bool ud){
    auto [myBoard, enemyBoard] =  definePlayerEnemy(pos);
    Bitboard completeBoard = redPieces | whitePieces;
    
    Bitboard ady = 0;
    Bitboard suc = 0;

    ady =  ud? rl ? ((pos & NOT_7_FILE) << 9) : ((pos & NOT_0_FILE) << 7):
           rl ? ((pos & NOT_7_FILE) >> 7) : ((pos & NOT_0_FILE) >> 9);

    
    suc = ud? rl ? ((ady & NOT_7_FILE) << 9) : ((ady & NOT_0_FILE) << 7):
           rl ? ((ady & NOT_7_FILE) >> 7) : ((ady & NOT_0_FILE) >> 9);


    if(ady == 0 || suc == 0) return false;
    
    if((ady & *enemyBoard) && (suc & completeBoard) == 0){
        int* enemyNum = (*enemyBoard&redPieces)!=0? &redPieceNum : &whitePieceNum;
        (*enemyNum)--;
        if((ady& queenPieces)){
            int* enemyKingNum = (*enemyBoard&redPieces)!= 0? &redKingNum : &whiteKingNum;
            (*enemyKingNum)--;
            queenPieces &= ~ady;
        }
        *myBoard &= ~pos;
        *myBoard |= suc;
        *enemyBoard &=~ady;
        

        

        if((pos&queenPieces)== 0){
            int r = __builtin_ctzll(suc) / 8;
            
            if(r == 0 || r == 7){
                queenPieces |= suc;
                int* myKingNum = (*myBoard&redPieces)!= 0? &redKingNum : &whiteKingNum;
                (*myKingNum)++;
            }
            if(canJump(suc,rl,ud)||
               canJump(suc,!rl,ud)){
                return 2;
            }
        }
        else{
            queenPieces &= ~pos;
            queenPieces |= suc;
            if(canJump(suc,rl,ud)||
               canJump(suc,!rl,ud)||
               canJump(suc,rl,!ud)||
               canJump(suc,!rl,!ud)){
                return 2;
            }   
        }
        return 1;
    }
    
    return 0;
}

bool Board::canJump(int r, int c, bool rl,bool ud){
    Bitboard pos = 1ULL<<(c+ 8*r);
    return canJump(pos,rl,ud);
}

bool Board::canJump(Bitboard pos, bool rl,bool ud){

    auto [myBoard, enemyBoard] =  definePlayerEnemy(pos);
    Bitboard completeBoard = redPieces | whitePieces;

    Bitboard ady = 0;
    Bitboard suc = 0;

    ady =  ud? rl ? ((pos & NOT_7_FILE) << 9) : ((pos & NOT_0_FILE) << 7):
           rl ? ((pos & NOT_7_FILE) >> 7) : ((pos & NOT_0_FILE) >> 9);
           
    suc = ud? rl ? ((ady & NOT_7_FILE) << 9) : ((ady & NOT_0_FILE) << 7):
           rl ? ((ady & NOT_7_FILE) >> 7) : ((ady & NOT_0_FILE) >> 9);


    if(ady == 0 || suc == 0) return false;

    return ((ady & *enemyBoard)!= 0 && (suc & completeBoard) == 0);
}



int Board::movePiece(int r, int c,bool rl){

    Bitboard pos = 1ULL<<(c+ 8*r);

    return movePiece(pos,rl);
}
int Board::movePiece(Bitboard pos,bool rl){
    if(!locatePiece(pos)){
        return false;
    }

    if((redPieces&pos) != 0){
        return movePiece(pos,rl,true);
    }
    else{
        return movePiece(pos,rl,false);
    }
}

bool Board::canMove(Bitboard pos, bool rl, bool ud){
    if ((pos & queenPieces) == 0) {
        if ((pos & redPieces) && !ud) return false;
        if ((pos & whitePieces) && ud) return false;
    }
    Bitboard completeBoard = redPieces | whitePieces;
    
    Bitboard newPos = ud? rl ? ((pos & NOT_7_FILE) << 9) : ((pos & NOT_0_FILE) << 7):
                          rl ? ((pos & NOT_7_FILE) >> 7) : ((pos & NOT_0_FILE) >> 9);
    if(newPos == 0) return false;

    return  (completeBoard & newPos)== 0;
}

// rl = true si vas a la derecha
// ud = true si vas arriba
int Board::movePiece(int r, int c, bool rl, bool ud){
    
    Bitboard pos = 1ULL<<(c+8*r);
    return movePiece(pos,rl,ud);
    
}

int Board::movePiece(Bitboard pos, bool rl, bool ud){
    if(!locatePiece(pos)) return false;
    auto [myBoard, enemyBoard] =  definePlayerEnemy(pos);
    
    Bitboard newPos = ud? rl ? ((pos & NOT_7_FILE) << 9) : ((pos & NOT_0_FILE) << 7):
                          rl ? ((pos & NOT_7_FILE) >> 7) : ((pos & NOT_0_FILE) >> 9);
    
    if(canJump(pos,rl,ud)) {
        return jump(pos,rl,ud);
    }
    
    if((newPos & *myBoard)!= 0 ||(newPos & *enemyBoard)!= 0 ) return false;
    
    if(newPos == 0) return false;
    if((pos& queenPieces) != 0){
        queenPieces &= ~pos;
        queenPieces |= newPos;
    }
    *myBoard &= ~pos;
    *myBoard |= newPos;
    int r = __builtin_ctzll( newPos)/8;
    if((r == 0 || (r == 7))&& (queenPieces& newPos) == 0){
        queenPieces |= newPos;
        int* myKingNum = (*myBoard&redPieces)!= 0? &redKingNum : &whiteKingNum;
        (*myKingNum)++;
    }
    return true;
}

string Board::print(){
    string board;
    Bitboard rpieces = redPieces;
    Bitboard wpieces = whitePieces;
    Bitboard queen = queenPieces;
    
    for(int i = 0; i < 8; i++){
        board+=to_string(i);
        for(int j = 0; j < 8; j++){
            if(rpieces%2 == 1 && queen%2==1){
            board += "|X";
            }
            else if (wpieces%2 == 1 && queen%2==1)
            {
                board += "|O";
            }
            else if (rpieces%2 ==1){
                board += "|x";
            }
            else if (wpieces%2 ==1){
                board += "|o";
            }
            else{
                board += "| ";
            }

            rpieces>>= 1;
            wpieces>>= 1;
            queen>>= 1;

        }
        board+= "|\n";
    }
    board+="  0 1 2 3 4 5 6 7";
    

    return board;
}

string Board::print(Bitboard bitboard){
    string board;
    for(int i = 0; i < 8; i++){
        board+=to_string(i);
        for(int j = 0; j < 8; j++){
            if(bitboard%2 == 1){
                board += "|#";
            }
            else{
                board += "| ";
            }
            bitboard>>= 1;
        }
        board+= "|\n";
    }
    board+="  0 1 2 3 4 5 6 7";
    return board;
}

Bitboard Board::getQueenPieces(){
    return queenPieces;
}

Bitboard Board::getWhitePieces(){
    return whitePieces;
}
Bitboard Board::getRedPieces(){
    return redPieces;
}


int Board::getWhiteNumPieces(){return whitePieceNum;}
int Board::getRedNumPieces(){return redPieceNum;}
int Board::getWhiteKingNumPieces(){return whiteKingNum;}
int Board::getRedKingNumPieces(){return redKingNum;}

BoardState Board::getAllBoards(){
    return {whitePieces,redPieces,queenPieces};
}

void Board::setAllBoards(BoardState boards, int whiteNum, int redNum, int wKnum,int rKNum){
    whitePieces = boards.white;
    redPieces = boards.red;
    queenPieces = boards.queens;
    whitePieceNum = whiteNum;
    redPieceNum = redNum;
    whiteKingNum = wKnum;
    redKingNum = rKNum;
}
