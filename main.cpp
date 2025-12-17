
#include <iostream>
#include "board/board.h"
using namespace std;

int main(){

    Board board;

    cout << board.print() << endl;

    board.movePiece(2,3, true);
    cout << board.print() << endl;
    board.movePiece(5,2, true);
    cout << board.print() << endl;
    board.movePiece(3,4, false);
    cout << board.print() << endl;
}