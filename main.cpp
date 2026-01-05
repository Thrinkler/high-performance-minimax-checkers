
#include <iostream>
#include "board/board.h"
#include "board/gameMaster.h"

#include "bots/minimax/minimax.h"
using namespace std;


void showBoard(GameMaster& gameMaster, Board& board){
    cout << (gameMaster.getPlayerPlaying()? "White player": "Red Player") <<endl;
    cout << board.print() << endl;
}


pair <int, int> translatePos(int pos){
    int r = 7 - (pos-1)/4;
    int v = pos%4 ? 8-2*(pos%4): 0;
    int c = (pos-1)%8>3? v+1 : v;
    cout<< r << " " << c << endl;
    return {r,c};
}

pair <string, string> translateNewPos(int pos1, int pos2){
    pair<int,int> where = translatePos(pos1);
    pair<int,int> to = translatePos(pos2);
    string rl = (where.second - to.second)> 0 ? "L" : "R";
    string ud = (where.first - to.first > 0) ? "U" : "D";
    cout << rl << " " << ud << endl;
    return {rl,ud};
}



void continuousGame(GameMaster& gameMaster, Board& board){
    string leftRight;
    string upDown;
    int pos, pos2;
    cout << "Posición?" << endl;
    cin >> pos;
    pair<int,int> tras = translatePos(pos);
    cout << "a donde lo quieres mover?" << endl;
    cin >> pos2;
    pair<string, string> move = translateNewPos(pos,pos2);

    if(pos == -1 || leftRight == "UNDO"){
        gameMaster.undoMove();
        gameMaster.undoMove();
        gameMaster.undoMove();
    }
    else gameMaster.movePiece(tras.first,tras.second,move.first == "R",move.second == "D");
}



void playMinMax(Board& board, GameMaster &gm, Minimax& min,bool player){
    
    pair<int,Move> move0 = min.minimax(gm,player);
    gm.movePiece(move0.second.pos,move0.second.rl,move0.second.ud);
    cout<< "Minimax valuation: "<<move0.first<< endl;
    cout<< "Move made: " << move0.second.str()<<endl;
    
}

void playMinVMin(Board &board, GameMaster& gameMaster, Minimax& minmax){
    for(int i = 0; i < 200; i++){
        cout << "----------Turn "<< i << "-----------" << endl;
        cout << "W: " << board.getWhiteNumPieces() << " R: " << board.getRedNumPieces() << endl;
        cout << "WK: " << board.getWhiteKingNumPieces() << " RK: " << board.getRedKingNumPieces() << endl;
        showBoard(gameMaster,board);
        string playName = gameMaster.getPlayerPlaying()? "white": "red";
        cout << "Board evaluation for "<< playName << " is " 
        << minmax.evaluateBoard(gameMaster,gameMaster.getPlayerPlaying()) << endl;
        playMinMax(board,gameMaster,minmax,gameMaster.getPlayerPlaying());
        cout << "---------------------" << endl;
        if(gameMaster.whoWon() != 0) break;
    }
    showBoard(gameMaster,board);
}

void playVMinMax(Board &board, GameMaster& gameMaster, Minimax& minmax){
    for(int i = 1; i < 300; i+=2){
        cout << "----------Turn "<< i << "-----------" << endl;
        cout << "W: " << board.getWhiteNumPieces() << " R: " << board.getRedNumPieces() << endl;
        cout << "WK: " << board.getWhiteKingNumPieces() << " RK: " << board.getRedKingNumPieces() << endl;
        showBoard(gameMaster,board);
        string playName = gameMaster.getPlayerPlaying()? "white": "red";
        cout << "Board evaluation for "<< playName << " is " 
        << minmax.evaluateBoard(gameMaster,gameMaster.getPlayerPlaying()) << endl;
        playMinMax(board,gameMaster,minmax,gameMaster.getPlayerPlaying());
        cout << "---------------------" << endl;
        if(gameMaster.whoWon() != 0) break;
        cout << "----------Turn "<< i +1<< "-----------" << endl;
        cout << "W: " << board.getWhiteNumPieces() << " R: " << board.getRedNumPieces() << endl;
        cout << "WK: " << board.getWhiteKingNumPieces() << " RK: " << board.getRedKingNumPieces() << endl;
        showBoard(gameMaster,board);
        playName = gameMaster.getPlayerPlaying()? "white": "red";
        cout << "Board evaluation for "<< playName << " is " 
        << minmax.evaluateBoard(gameMaster,gameMaster.getPlayerPlaying()) << endl;
        continuousGame(gameMaster,board);
        cout << "---------------------" << endl;
        if(gameMaster.whoWon() != 0) break;
        
        
    }
    showBoard(gameMaster,board);
}


int main(){

    Board board;

    GameMaster gameMaster(&board,true);
    cout<<gameMaster.print((1ULL<<16)-1)<< endl;
    cout<< gameMaster.print(((1ULL<<16)-1)<<6*8)<<endl;
    

    Minimax minmax = Minimax(12);

    cout << "Initial Board:" << endl;
    playMinVMin(board,gameMaster,minmax);
    //playVMinMax(board,gameMaster,minmax);

}
