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

    int udTo = ud? 1:-1;
    int rlTo = rl? 1:-1;
    
    Bitboard ady = 0;
    Bitboard suc = 0;

    ady = ud?((pos<<(rl?9:7)) & (rl? NOT_0_FILE:NOT_7_FILE)):
             ((pos>>(rl?7:9)) & (rl? NOT_0_FILE:NOT_7_FILE));

    
    suc = ud?((pos<<(rl?18:14)) & (rl? NOT_0_FILE:NOT_7_FILE)):
             ((pos>>(rl?14:18)) & (rl? NOT_0_FILE:NOT_7_FILE));


    if(ady == 0 || suc == 0) return false;

    if((ady & *enemyBoard) && (suc & completeBoard) == 0){
        *myBoard &= ~pos;
        *myBoard |= suc;
        *enemyBoard &=~ady;
        int* enemyNum = (*enemyBoard&redPieces)? &redPieceNum : &whitePieceNum;

        (*enemyNum)--;

        if((ady& queenPieces)){
            queenPieces &= ~ady;
            int* enemyKingNum = (*enemyBoard&redPieces)? &redKingNum : &whiteKingNum;
            (*enemyKingNum)--;
        }

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

    ady = ud?((pos<<(rl?9:7)) & (rl? NOT_0_FILE:NOT_7_FILE)):
             ((pos>>(rl?7:9)) & (rl? NOT_0_FILE:NOT_7_FILE));

    
    suc = ud?((pos<<(rl?18:14)) & (rl? NOT_0_FILE:NOT_7_FILE)):
             ((pos>>(rl?14:18)) & (rl? NOT_0_FILE:NOT_7_FILE));


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
    Bitboard completeBoard = redPieces | whitePieces;
    
    Bitboard newPos = ud?((pos<<(rl?9:7)) & (rl? NOT_0_FILE:NOT_7_FILE)):
                      ((pos>>(rl?7:9)) & (rl? NOT_0_FILE:NOT_7_FILE));

    return  (completeBoard| newPos)== 0;
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
    
    Bitboard newPos = ud?((pos<<(rl?9:7)) & (rl? NOT_0_FILE:NOT_7_FILE)):
             ((pos>>(rl?7:9)) & (rl? NOT_0_FILE:NOT_7_FILE));
    
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
    if(r+(ud? 1 : -1) == 0 || (r+(ud? 1 : -1) == 7)){
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

BoardState Board::getAllBoards(){
    return {whitePieces,redPieces,queenPieces};
}