#include "gameMaster.h"
#include<iostream>
using namespace std;

const Bitboard NOT_0_FILE = 0xfefefefefefefefeULL;
const Bitboard NOT_7_FILE = 0x7f7f7f7f7f7f7f7fULL;


string GameMaster::print(Bitboard bitboard){
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
            bitboard/=2;
        }
        board+= "|\n";
    }
    board+="  0 1 2 3 4 5 6 7";
    return board;
}


GameMaster::GameMaster(){
    board = new Board();
    player = true; //1 si es el jugador blanco, 0 si es rojo.
    iOwnBoard = true;
}
GameMaster::GameMaster(Bitboard whitePieces, Bitboard redPieces, Bitboard queenPieces, bool playerToPlay){
    board = new Board(whitePieces,redPieces,queenPieces);
    player = playerToPlay;
    iOwnBoard = true;
}
GameMaster::GameMaster(BoardState boardstate, bool playerToPlay){
    board = new Board(boardstate.white,boardstate.red,boardstate.queens);
    player = playerToPlay;
    iOwnBoard = true;
}
GameMaster::GameMaster(Board* b, bool playerToPlay){ 
    board = b;
    player = playerToPlay; 
    iOwnBoard = false;
}

GameMaster::~GameMaster() {
    if (iOwnBoard && board != nullptr) {
        delete board;
    }
}

pair<Bitboard,Bitboard> GameMaster::definePlayerEnemy(){
    if(player){
        return {board->getWhitePieces(),board->getRedPieces()};
    }
    return {board->getRedPieces(),board->getWhitePieces()};
}


Bitboard GameMaster::getWhiteMoves(){
    Bitboard white = board->getWhitePieces();
    Bitboard red = board->getRedPieces();
    Bitboard emptySpaces = ~(white | red);

    Bitboard queens = (board -> getQueenPieces())& white;

    Bitboard rightAdy = (white & NOT_7_FILE)>> 7;
    Bitboard leftAdy = (white & NOT_0_FILE)>> 9;
    Bitboard qRightAdy = (queens & NOT_7_FILE)<< 9;
    Bitboard qLeftAdy = (queens & NOT_0_FILE)<< 7;
    
    Bitboard attacksRight = (((rightAdy& red) & NOT_7_FILE)>>7)&emptySpaces;
    Bitboard attacksLeft = (((leftAdy& red) & NOT_0_FILE)>>9)&emptySpaces;
    Bitboard qAttacksRight = (((qRightAdy& red) & NOT_7_FILE)<<9)&emptySpaces;
    Bitboard qAttacksLeft = (((qLeftAdy& red) & NOT_0_FILE)<<7)&emptySpaces;
    
    Bitboard rightAttack = (((attacksRight& emptySpaces)<<14) | ((qAttacksRight & emptySpaces)>>18));
    Bitboard leftAttack = (((attacksLeft& emptySpaces)<<18) | ((qAttacksLeft & emptySpaces)>>14));

    Bitboard pieceRight;
    Bitboard pieceLeft;
    if((rightAttack | leftAttack) != 0){
        pieceRight = rightAttack;
        pieceLeft = leftAttack;
    }
    else{

        pieceRight = (rightAdy  & emptySpaces)<<7;
        pieceRight |= (qRightAdy & emptySpaces) >>9;
        pieceLeft = (leftAdy & emptySpaces)<<9;
        pieceLeft |= (qLeftAdy & emptySpaces) >>7;
    }
    
    return pieceRight| pieceLeft;
}

Bitboard GameMaster::getRedMoves(){
    Bitboard white = board->getWhitePieces();
    Bitboard red = board->getRedPieces();
    Bitboard emptySpaces = ~(white | red);

    Bitboard queens = (board->getQueenPieces())&red;

    Bitboard rightAdy = (red & NOT_7_FILE)<< 9;
    Bitboard leftAdy = (red & NOT_0_FILE)<< 7;
    Bitboard qRightAdy = (queens & NOT_7_FILE)>> 7;
    Bitboard qLeftAdy = (queens & NOT_0_FILE)>> 9;

    Bitboard attacksRight = (((rightAdy& white) & NOT_7_FILE)<<9)&emptySpaces;
    Bitboard attacksLeft = (((leftAdy& white) & NOT_0_FILE)<<7)&emptySpaces;
    Bitboard qAttacksRight = (((qRightAdy& white) & NOT_7_FILE)>>7)&emptySpaces;
    Bitboard qAttacksLeft = (((qLeftAdy& white) & NOT_0_FILE)>>9)&emptySpaces;

    Bitboard rightAttack = (((attacksRight& emptySpaces)>>18) | ((qAttacksRight & emptySpaces)<<14));
    Bitboard leftAttack = (((attacksLeft& emptySpaces)>>14) | ((qAttacksLeft & emptySpaces)<<18));

    Bitboard pieceRight;
    Bitboard pieceLeft;
    if((rightAttack | leftAttack) != 0){
        pieceRight =rightAttack;
        pieceLeft = leftAttack;
    }
    else{
        pieceRight = (rightAdy  & emptySpaces)>>9;
        pieceRight |= (qRightAdy & emptySpaces) <<7;
        pieceLeft = (leftAdy & emptySpaces)>>7;
        pieceLeft |= (qLeftAdy & emptySpaces) <<9;
    }
    return pieceRight| pieceLeft;
}

bool GameMaster::movePiece(int r, int c, bool rl, bool ud){
    Bitboard validMoves = player? getWhiteMoves(): getRedMoves();
    Bitboard pos = 1ULL<<(c+ 8*r);
    if((validMoves & pos)== 0) return false;

    bool isJump = board->canJump(r,c,rl,ud);
    if(!isJump){
        if(board->canJump(r,c,!rl,ud)) return false;
        if((pos & board -> getQueenPieces())!= 0){
            if(board->canJump(r,c,rl,!ud)|| board->canJump(r,c,!rl,!ud)) return false;
        }
    }

    int turnContinues = board->movePiece(r,c,rl,ud);
    if(turnContinues == 0) return false;

    if(turnContinues == 1) player = !player;
    

    return true;
}

void GameMaster::addLastMove(int r, int c, bool rl, bool ud){

}

bool GameMaster::undoMove(){
    
}


vector<Move> GameMaster::getPossibleMoves(){
    Bitboard validMoves = player? getWhiteMoves(): getRedMoves();
    vector<Move> out;
    out.reserve(20);
    Bitboard move;

    while(validMoves){
        move = 1ULL<<__builtin_ctzll(validMoves);
        
        if((board ->canMove(move,RIGHT,UP) || board->canJump(move,RIGHT,UP))){
            out.emplace_back(move,RIGHT,UP);
        }
        if((board ->canMove(move,LEFT,UP) || board->canJump(move,LEFT,UP))){
            out.emplace_back(move,LEFT,UP);
        }
        if((board ->canMove(move,RIGHT,DOWN) || board->canJump(move,RIGHT,DOWN))){
            out.emplace_back(move,RIGHT,DOWN);
        }
        if((board ->canMove(move,LEFT,DOWN) || board->canJump(move,LEFT,DOWN))){
            out.emplace_back(move,LEFT,DOWN);
        }

        validMoves &= (validMoves - 1);
    }
    return out;
}


bool GameMaster::getPlayerPlaying(){
    return player;
}


