#include "minimax.h"
#include<iostream>

using namespace std;

const int VALUE_WIN = 100000000;
const Move emptyMove = Move(0,0,0);

Minimax::Minimax(int depth) {
    maxDepth = depth;
}
int Minimax::evaluateBoard(GameMaster& gm, int turn, int depth){
    BoardState boards = gm.board -> getAllBoards();
    if(umap.find(boards) != umap.end()){
        if(umap.at(boards)> 2){
            return -10000-umap.at(boards);
        }
    }

    if(gm.whoWon() != 0){
        if(gm.whoWon() == 2) return 0;
        int valuation = (VALUE_WIN) * gm.whoWon() * turn;
        if(valuation == abs(valuation)){
            valuation -= (maxDepth - depth);
        }
        else{
            valuation -= depth;
        }
        return valuation;
    }
    
    int valMen = (gm.board ->getWhiteNumPieces() - gm.board->getRedNumPieces())*100;
    int valKings = (gm.board->getWhiteKingNumPieces() - gm.board->getRedKingNumPieces())*1000;

    return (valMen + valKings) * turn;
}

int Minimax::evaluateBoard(GameMaster& gm, bool player, int depth){
    int turn = player? 1:-1;
    return evaluateBoard(gm,turn,depth);
}

pair<int,Move> Minimax::minimax(GameMaster& gm, bool player){
    string playName = player? "white, depth = ": "red, depth = ";
    cout<< "Playing as:" << playName<<maxDepth << endl;
    BoardState boards = gm.board -> getAllBoards();
    if(umap.find(boards) == umap.end()){
        umap.emplace(boards,1);
    }
    else umap.at(boards)++;

    return minimax(gm,maxDepth,-VALUE_WIN,VALUE_WIN,player);
}

pair<int,Move> Minimax::minimax(GameMaster& gm, int depth, int alpha, int beta, bool player){
    int boardVal = player? 1:-1;
    
    if(depth == 0 || gm.whoWon() != 0){
        return {evaluateBoard(gm, boardVal, depth),emptyMove};
    }
    
    vector<Move> possibleMoves = gm.getPossibleMoves();
    
    if(!possibleMoves.size()) return  {evaluateBoard(gm,  boardVal, depth),emptyMove};
    int score;
    int bestScore = -VALUE_WIN;
    Move sucMove = Move(0,0,0);
    Move bestMove = possibleMoves[0];

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(possibleMoves), std::end(possibleMoves), rng);

    BoardState boards = gm.board -> getAllBoards();
    if(umap.find(boards) == umap.end()){
        umap.emplace(boards,1);
    }
    else umap.at(boards)++;
    if(umap.at(boards) > 2 && depth < maxDepth){
        return {evaluateBoard(gm, boardVal, depth), possibleMoves[0]};
    }

    for(auto move : possibleMoves){
        bool moved = gm.movePiece(move.pos,move.rl,move.ud);
        bool changeP = gm.getPlayerPlaying() == player;
        if(gm.getPlayerPlaying() != player){
            score = -minimax(gm,depth-1,  -beta, -alpha, gm.getPlayerPlaying()).first;
        }
        else{
            score = minimax(gm,depth,  alpha, beta, gm.getPlayerPlaying()).first;
        }
        if (moved)gm.undoMove();
        else{
            cout<< "error del movimiento " << move.str() << endl;
        }

        if(score > bestScore){
            bestScore = score;
            bestMove = move;
        }

        if(bestScore >= alpha){
            alpha = bestScore;
        }
       
        if(alpha >= beta){
            break;
        }
    }
    umap.at(boards)--;
    return {bestScore,bestMove};

}