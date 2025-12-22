
#include <iostream>
#include "board/board.h"
#include "board/gameMaster.h"

#include "bots/minimax/minimax.h"
using namespace std;


string print(Bitboard bitboard){
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

void showBoard(GameMaster& gameMaster, Board& board){
    cout << (gameMaster.getPlayerPlaying()? "White player": "Red Player") <<endl;
    //cout<< "possible pieces to move:"<< endl;
    //cout << print(gameMaster.getPlayerPlaying()?gameMaster.getWhiteMoves():gameMaster.getRedMoves())<< endl;
    //cout<< "possible Moves:"<< endl;
    /*
    vector<Move> possibleMoves = gameMaster.getPossibleMoves();

    for(auto move: possibleMoves){
        cout << move.str() << ",";
    }cout<< endl;
    */
    cout << board.print() << endl;
}


void game1(GameMaster& gameMaster, Board& board){
    gameMaster.movePiece(2,1,RIGHT,DOWN);
    showBoard(gameMaster,board);

    gameMaster.movePiece(5,4,LEFT,UP);
    showBoard(gameMaster,board);
    
    gameMaster.movePiece(3,2,RIGHT,DOWN);
    showBoard(gameMaster,board);

    gameMaster.movePiece(6,3,RIGHT,UP);
    showBoard(gameMaster,board);

    gameMaster.movePiece(2,3,LEFT,DOWN);
    showBoard(gameMaster,board);

    gameMaster.movePiece(5,0,RIGHT,UP);
    showBoard(gameMaster,board);

    gameMaster.movePiece(3,2,LEFT,DOWN);
    showBoard(gameMaster,board);
    gameMaster.movePiece(7,2,RIGHT,UP);
    showBoard(gameMaster,board);

    gameMaster.movePiece(5,0,RIGHT,DOWN);
    showBoard(gameMaster,board);
}

void continuousGame(GameMaster& gameMaster, Board& board){
    string leftRight;
    string upDown;
    int r,c;
    cout << "Coordenadas?" << endl;
    cin >> r >> c;
    cout<< "Qué movimiento quieres? L:R Arriba o abajo? U:D" << endl; 
    cin >> leftRight >> upDown;

    if(r == -1 || leftRight == "UNDO"){
        gameMaster.undoMove();
        gameMaster.undoMove();
        gameMaster.undoMove();
    }
    else gameMaster.movePiece(r,c,leftRight == "R",upDown == "D");
    
}

void playMinMax(Board& board, GameMaster &gm, Minimax& min,bool player){
    
    pair<int,Move> move0 = min.minimax(gm,player);
    gm.movePiece(move0.second.pos,move0.second.rl,move0.second.ud);
    cout<< "Minimax valuation: "<<move0.first<< endl;
    cout<< "Move made: " << move0.second.str()<<endl;
    
}

void playMinVMin(Board &board, GameMaster& gameMaster, Minimax& minmax){
    for(int i = 0; i < 200; i++){
        cout << "---------------------" << endl;
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
    for(int i = 0; i < 100; i++){
        cout << "---------------------" << endl;
        cout << "W: " << board.getWhiteNumPieces() << " R: " << board.getRedNumPieces() << endl;
        cout << "WK: " << board.getWhiteKingNumPieces() << " RK: " << board.getRedKingNumPieces() << endl;
        showBoard(gameMaster,board);
        string playName = gameMaster.getPlayerPlaying()? "white": "red";
        cout << "Board evaluation for "<< playName << " is " 
        << minmax.evaluateBoard(gameMaster,gameMaster.getPlayerPlaying()) << endl;
        continuousGame(gameMaster,board);
        cout << "---------------------" << endl;
        if(gameMaster.whoWon() != 0) break;
        cout << "---------------------" << endl;
        cout << "W: " << board.getWhiteNumPieces() << " R: " << board.getRedNumPieces() << endl;
        cout << "WK: " << board.getWhiteKingNumPieces() << " RK: " << board.getRedKingNumPieces() << endl;
        showBoard(gameMaster,board);
        playName = gameMaster.getPlayerPlaying()? "white": "red";
        cout << "Board evaluation for "<< playName << " is " 
        << minmax.evaluateBoard(gameMaster,gameMaster.getPlayerPlaying()) << endl;
        playMinMax(board,gameMaster,minmax,gameMaster.getPlayerPlaying());
        cout << "---------------------" << endl;
        if(gameMaster.whoWon() != 0) break;
        
    }
    showBoard(gameMaster,board);
}


int main(){

    Board board;

    GameMaster gameMaster(&board,false);

    Minimax minmax = Minimax(15);

    cout << "Initial Board:" << endl;
    playMinVMin(board,gameMaster,minmax);
    //playVMinMax(board,gameMaster,minmax);
    //test(gameMaster, board);
    
    
    
    //playMinMax(board,gameMaster,minmax,gameMaster.getPlayerPlaying());
    

    //game1(gameMaster,board);

    //continuousGame(gameMaster,board);

}