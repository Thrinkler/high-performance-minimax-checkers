#include "board.h"
#include<iostream>
using namespace std;

using BoardPair = std::pair<Bitboard&, Bitboard&>;

Board::Board(){
    whitePieces = 0;
    redPieces = 0;
    queenPieces = 0;

    for(int i = 0; i < 4; i++){
        redPieces = redPieces | 1ULL<<(2*i+1);
        redPieces = redPieces | (1ULL<<(2*i+8));
        redPieces = redPieces | (1ULL<<(2*i+17));
        
        whitePieces = whitePieces | 1ULL<<(2*i+ 8*7);
        whitePieces = whitePieces | 1ULL<<(2*i+ 8*6+1);
        whitePieces = whitePieces | 1ULL<<(2*i+ 8*5);
    }
}
bool Board::locatePiece(int r, int c){
    Bitboard completeBoard = redPieces | whitePieces;

    Bitboard pos = 1ULL<<(c+ 8*r);

    return (completeBoard&pos) != 0;
}

pair<Bitboard*,Bitboard*> Board::definePlayerEnemy(int r, int c){
    Bitboard pos = 1ULL<<(c+ 8*r);

    if ((redPieces & pos) != 0) {
        return { &redPieces, &whitePieces }; 
    }
    else if ((whitePieces & pos) != 0) {
        return { &whitePieces, &redPieces };
    }
    return  {nullptr, nullptr };
}



bool Board::jump(int r, int c,bool rl, bool ud){
    Bitboard pos = 1ULL<<(c+ 8*r);
    auto [myBoard, enemyBoard] =  definePlayerEnemy(r,c);
    Bitboard completeBoard = redPieces | whitePieces;

    Bitboard ady = 0;
    Bitboard suc = 0;


    if(r-2 >= 0 && !ud){
        if (c+2 <8 && rl)  {
            ady |= 1ULL<<(c+1 + 8*(r-1));
            suc |= 1ULL<<(c+2 + 8*(r-2));
        }
        if (c-2 >-1 && !rl) {
            ady |= 1ULL<<(c-1 + 8*(r-1));
            suc |= 1ULL<<(c-2 + 8*(r-2));
        }

    }
    if(r+2 < 8 && ud){
        if (c+2 <8 && rl)  {
            ady |= 1ULL<<(c+1 + 8*(r+1));
            suc |= 1ULL<<(c+2 + 8*(r+2));
        }
        if (c-2 >-1 && !rl) {
            ady |= 1ULL<<(c-1 + 8*(r+1));
            suc |= 1ULL<<(c-2 + 8*(r+2));
        }
    }
    if((ady & *enemyBoard) && (suc & completeBoard) == 0){
        *myBoard &= ~pos;
        *myBoard |= suc;
        if((pos& queenPieces) != 0){
            queenPieces &= ~pos;
            queenPieces |= suc;
        }
        
        *enemyBoard &=~ady;
        if((ady& queenPieces) != 0){
            queenPieces &= ~ady;
        }

        if(r+(ud? 2 : -2) == 0 || (r+(ud? 2 : -2) == 7)){
            queenPieces |= suc;
        }
        return true;
    }
    
    return false;
}

bool Board::canJump(int r, int c, bool rl){
    Bitboard pos = 1ULL<<(c+ 8*r);
    auto [myBoard, enemyBoard] =  definePlayerEnemy(r,c);
    Bitboard completeBoard = redPieces | whitePieces;
    Bitboard upAdy = 0;
    Bitboard dwAdy = 0;

    Bitboard upSuc = 0;
    Bitboard dwSuc = 0;
    if(r-2 >= 0 ){
        if (c+2 <8 && rl)  {
            upAdy |= 1ULL<<(c+1 + 8*(r-1));
            upSuc |= 1ULL<<(c+2 + 8*(r-2));
        }
        if (c-2 >-1 && !rl) {
            upAdy |= 1ULL<<(c-1 + 8*(r-1));
            upSuc |= 1ULL<<(c-2 + 8*(r-2));
        }
    }
    if(r+2 < 8 ){
        if (c+2 <8 && rl)  {
            dwAdy |= 1ULL<<(c+1 + 8*(r+1));
            dwSuc |= 1ULL<<(c+2 + 8*(r+2));
        }
        if (c-2 >-1 && !rl) {
            dwAdy |= 1ULL<<(c-1 + 8*(r+1));
            dwSuc |= 1ULL<<(c-2 + 8*(r+2));
        }
    }
    
    bool ret = false;
    if((pos&redPieces)!= 0 || (pos&queenPieces) != 0){
        if((dwAdy & *enemyBoard)!= 0 && (dwSuc & completeBoard) == 0){
            ret = true;
        }
    }
    if((pos&whitePieces)!= 0 || (pos&queenPieces) != 0){
        if((upAdy & *enemyBoard)!= 0 && (upSuc & completeBoard) == 0){
            ret = true;
        }
    }

    return ret;
}



bool Board::movePiece(int r, int c,bool rl){
    if(!locatePiece(r,c)){
        return false;
    }

    Bitboard pos = 1ULL<<(c+ 8*r);

    if((redPieces&pos) != 0){
        return movePiece(r,c,rl,true);
    }
    else{
        return movePiece(r,c,rl,false);
    }
}

// rl = true si vas a la derecha
// ud = true si vas arriba
bool Board::movePiece(int r, int c, bool rl, bool ud){
    if(!locatePiece(r,c)) return false;
    auto [myBoard, enemyBoard] =  definePlayerEnemy(r,c);
    Bitboard pos = 1ULL<<(c+8*r);
    Bitboard newPos = 1ULL<<(c + (rl? 1 : -1) + 8*(r+(ud? 1 : -1)));
    
    if(canJump(r,c,rl)) return jump(r,c,rl,ud);
    
    if((newPos & *myBoard)!= 0 ||(newPos & *enemyBoard)!= 0 ) return false;

    if((c == 0 && rl == false) || (c == 7 && rl == true)) return false;
        if((r == 0 && ud == true) || (r == 7 && ud == false)) return false;
        if((pos& queenPieces) != 0){
            queenPieces &= ~pos;
            queenPieces |= newPos;
        }
        *myBoard &= ~pos;
        *myBoard |= newPos;
        if(r+(ud? 1 : -1) == 0 || (r+(ud? 1 : -1) == 7)){
            queenPieces |= newPos;
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

            rpieces/=2;
            wpieces/=2;
            queen/=2;

        }
        board+= "|\n";
    }
    board+="  0 1 2 3 4 5 6 7";
    

    return board;
}