
#include <iostream>
#include "board/board.h"
#include "board/gameMaster.h"
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

void showBoard(GameMaster gameMaster, Board board){
    cout << (gameMaster.getPlayerPlaying()? "White player": "Red Player") <<endl;
    cout<< "possible pieces to move:"<< endl;
    cout << print(gameMaster.getPlayerPlaying()?gameMaster.getWhiteMoves()
                    :gameMaster.getRedMoves())<< endl;
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
    for(int i =0; i< 20; i++){
        cout<< print(board.getQueenPieces())<< endl;
        cout<<print(board.getRedPieces()) << endl;
        cout << print(board.getWhitePieces())<< endl;
        showBoard(gameMaster,board);
        string leftRight;
        string upDown;
        int r,c;
        cout << "Coordenadas?" << endl;
        cin >> r >> c;
        cout<< "Qué movimiento quieres? L:R Arriba o abajo? U:D" << endl; 
        cin >> leftRight >> upDown;

        gameMaster.movePiece(r,c,leftRight == "R",upDown == "D");
    }
}

int main(){

    Board board;
    GameMaster gameMaster(&board,false);
    
    showBoard(gameMaster,board);
    game1(gameMaster,board);

    cout<< print(board.getQueenPieces())<< endl;
    cout<<print(board.getRedPieces()) << endl;
    cout << print(board.getWhitePieces())<< endl;

    continuousGame(gameMaster,board);

    //gameMaster.getPossibleMoves();
    /*
    cout << board.print() << endl;
    
    cout << print(gameMaster.getRedMoves())<< endl;

    cout << "White turn---------" << endl;
    board.movePiece(2,3, true);
    cout << board.print() << endl;

    cout << print(gameMaster.getWhiteMoves())<< endl;
    cout << "Red turn---------" << endl;
    board.movePiece(5,2, true);
    cout << board.print() << endl;
    cout << print(gameMaster.getRedMoves())<< endl;
    cout << "White turn---------" << endl;
    board.movePiece(3,4, false);
    cout << board.print() << endl;
    cout << print(gameMaster.getWhiteMoves())<< endl;

    */
}