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
    if(multiJumpPiecePos != 0){
        return multiJumpPiecePos;
    }
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
    if(multiJumpPiecePos != 0){
        return multiJumpPiecePos;
    }
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
    Bitboard pos = 1ULL<<(c+ 8*r);
    return movePiece(pos,rl,ud);
}

bool GameMaster::movePiece(Bitboard pos, bool rl, bool ud){
    multiJumpPiecePos = 0;
    Bitboard validMoves = player? getWhiteMoves(): getRedMoves();

    if((validMoves & pos)== 0) return false;

    bool isJump = board->canJump(pos,rl,ud);
    if(!isJump){
        if(board->canJump(pos,!rl,ud)) return false;
        if((pos & board -> getQueenPieces())!= 0){
            if(board->canJump(pos,rl,!ud)|| board->canJump(pos,!rl,!ud)) return false;
        }
    }
    Snapshot snap = Snapshot(board->getAllBoards(), 
                             board->getWhiteNumPieces(), 
                             board->getRedNumPieces(),
                             board->getWhiteKingNumPieces(), 
                             board->getRedKingNumPieces());
    int turnContinues = board->movePiece(pos,rl,ud);
    if(turnContinues == 0) return false;

    snap.changedTurns = (turnContinues == 1);
    prevBoard.emplace(snap);

    if(turnContinues == 1) player = !player;
    else{
        multiJumpPiecePos = ud? rl ? ((pos & NOT_7_FILE) << 18) : ((pos & NOT_0_FILE) << 14):
                                rl ? ((pos & NOT_7_FILE) >> 14) : ((pos & NOT_0_FILE) >> 18);
    }
    return true;
}


bool GameMaster::undoMove(){
    if (prevBoard.empty()) return false;
    Snapshot lastBoard = prevBoard.top();
    prevBoard.pop();
    board->setAllBoards(lastBoard.boards, lastBoard.whiteNum, lastBoard.redNum,
                          lastBoard.whiteKNum, lastBoard.redKNum);
    multiJumpPiecePos = lastBoard.multijumpPiecePos;
    if(lastBoard.changedTurns){
        player = !player;
    }
    return true;
}


vector<Move> GameMaster::getPossibleMoves(){
    Bitboard validMoves = multiJumpPiecePos != 0? multiJumpPiecePos : (player? getWhiteMoves(): getRedMoves());
    vector<Move> out;
    out.reserve(20);
    Bitboard move;
   
    bool dirUD[] = {UP, DOWN};
    bool dirRL[] = {LEFT, RIGHT};
    
     bool canJ = false;
    while(validMoves){
        move = 1ULL<<__builtin_ctzll(validMoves);
        if(!(move & board ->getQueenPieces())){
            if(player) dirUD[1] = UP;
            else dirUD[0] = DOWN;
        }
       
        for(bool ud : dirUD) 
            for(bool rl : dirRL)
                if(board->canJump(move,rl,ud)){
                    out.emplace_back(move,rl,ud);
                    canJ = true;
                }
        
        if(!canJ)
            for(bool ud : dirUD)
                for(bool rl : dirRL)
                    if((board ->canMove(move,rl,ud))){
                        out.emplace_back(move,rl,ud);
                    }
        validMoves &= (validMoves - 1);
    }
    return out;
}

int GameMaster::numberOfMoves(){
    Bitboard validMoves = multiJumpPiecePos != 0? multiJumpPiecePos : (player? getWhiteMoves(): getRedMoves());
    int ret = 0;

    while(validMoves){
        ret++;
        validMoves &= (validMoves - 1);
    }
    return ret;
}

bool GameMaster::hasToJump(){
    Bitboard validMoves = multiJumpPiecePos != 0? multiJumpPiecePos : (player? getWhiteMoves(): getRedMoves());
    if(validMoves == 0) return false;
    Bitboard move = 1ULL<<__builtin_ctzll(validMoves);
    bool dirUD[] = {UP, DOWN};
    bool dirRL[] = {LEFT, RIGHT};
    
    if(!(move & board ->getQueenPieces())){
        if(player) dirUD[1] = UP;
        else dirUD[0] = DOWN;
    }
    for(bool ud : dirUD) 
            for(bool rl : dirRL)
                if(board->canJump(move,rl,ud)){
                    return true;
                }
    return false;

}

int GameMaster::whoWon(){
    if((board->getRedNumPieces() == 1 && board->getWhiteNumPieces() == 1)) return 2;
    if(board ->getRedNumPieces() == 0) return 1;
    if(board -> getWhiteNumPieces() == 0) return -1;
    return 0;
}

bool GameMaster::getPlayerPlaying(){
    return player;
}

bool GameMaster:: changePlayerPlaying(){
    player = !player;
    return player;
}
