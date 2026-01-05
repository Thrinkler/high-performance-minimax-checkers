#include "minimax.h"
#include<iostream>
#include <chrono>

using namespace std;

const int VALUE_WIN = 100000000;
const Move emptyMove = Move(0,0,0);
static std::random_device rd;
static std::mt19937 g(rd());
std::bernoulli_distribution should_shuffle(0.5);

Minimax::Minimax(int depth) :maxDepth(depth){}

int Minimax::evaluateBoard(GameMaster& gm, int turn, int depth){
    int mult = 100;

    if(gm.whoWon() != 0){
        if(gm.whoWon() == 2) return 0;
        int valuation = (VALUE_WIN + depth) * gm.whoWon() * turn;
        return valuation;
    }
    int whiteVal = gm.board ->getWhiteNumPieces() + gm.board->getWhiteKingNumPieces();
    int redVal = gm.board->getRedNumPieces() +  gm.board->getRedKingNumPieces();

    int valuation = turn> 0? whiteVal*mult / redVal : redVal*mult/whiteVal;
    int posNum = gm.numberOfMoves();
    gm.changePlayerPlaying();
    int opNum =  gm.numberOfMoves();
    gm.changePlayerPlaying();
    int movCramp = posNum - opNum;
    if(opNum != 0 && gm.hasToJump()) movCramp = 0;

    return (valuation-mult)*10 + movCramp + heatmapValuation(gm,turn);
}

int Minimax::heatmapValuation(GameMaster& gm, int turn){
    int ret = 0;
    if(turn > 0){
        Bitboard whiteMen = gm.board ->getWhitePieces() & (~ gm.board ->getQueenPieces());
        Bitboard last2Rows = (1ULL<<16)-1;
        if(whiteMen & last2Rows) ret++;
    }

    else{
        Bitboard redMen = gm.board ->getRedPieces() & (~ gm.board ->getQueenPieces());
        Bitboard first2Rows = ((1ULL<<16)-1)<<6*8;
        if(redMen & first2Rows) ret++;
    }

    return ret;
}

int Minimax::evaluateBoard(GameMaster& gm, bool player, int depth){
    int turn = player? 1:-1;
    return evaluateBoard(gm,turn,depth);
}

pair<int,Move> Minimax::minimax(GameMaster& gm, bool player){
    string playName = player? "white, depth = ": "red, depth = ";
    numIt = 0;
    cout<< "Playing as:" << playName<<maxDepth << endl;
    BoardState boards = gm.board -> getAllBoards();
    if(repetitionTable.find(boards) == repetitionTable.end()){
        repetitionTable.emplace(boards,1);
    }
    else repetitionTable.at(boards)++;
    auto start = std::chrono::high_resolution_clock::now();
    vector<Move> possibleMoves = gm.getPossibleMoves();
    if(!possibleMoves.size()) return  {-VALUE_WIN,emptyMove};
    pair<int,Move> firstMove = {evaluateBoard(gm, player, maxDepth),possibleMoves[0]};
    pair<int,Move> ret = possibleMoves.size()>1? minimax(gm,maxDepth,-VALUE_WIN-1000,VALUE_WIN+1000,player) : firstMove;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    cout << "Tiempo de pensamiento: " << duration.count() << " ms" << std::endl;
    double seconds = duration.count() / 1000.0;
    long long nps = (seconds > 0.0001) ? (long long)(numIt / seconds) : 0;
    cout << "Numero de iteraciones: " << numIt << std::endl;
    cout << "Numero de iteraciones por segundo: " << nps << std::endl;
    return ret;
}

pair<int,Move> Minimax::minimax(GameMaster& gm, int depth, int alpha, int beta, bool player){
    int boardVal = player? 1:-1;

    if(gm.whoWon() != 0){
        return {evaluateBoard(gm, boardVal, depth),emptyMove};
    }
    if(depth == 0){
        return quiescence(gm,alpha,beta,player);
    }
    numIt++;
    vector<Move> possibleMoves = gm.getPossibleMoves();

    if(!possibleMoves.size()) return  {-VALUE_WIN,emptyMove};
    int score;
    int bestScore = -VALUE_WIN;
    Move bestMove = possibleMoves[0];
    if (should_shuffle(g)) std::shuffle(std::begin(possibleMoves), std::end(possibleMoves), g);

    BoardState boards = gm.board -> getAllBoards();
    if(repetitionTable.find(boards) == repetitionTable.end()){
        repetitionTable.emplace(boards,1);
    }
    else repetitionTable.at(boards)++;
    if(repetitionTable.at(boards) > 2 && depth < maxDepth){
        repetitionTable.at(boards)--;
        return {0, possibleMoves[0]};
    }

    for(auto move : possibleMoves){
        bool moved = gm.movePiece(move.pos,move.rl,move.ud);
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

        alpha = std::max(alpha, bestScore);

        if(alpha >= beta){
            break;
        }
    }
    repetitionTable.at(boards)--;
    return {bestScore,bestMove};

}

pair<int,Move> Minimax::quiescence(GameMaster& gm, int alpha, int beta, bool player){
    numIt++;
    int boardVal = player? 1:-1;

    int boardValuation = evaluateBoard(gm, boardVal, 0);
    if(boardValuation >= beta) return {beta, emptyMove};

    if(! gm.numberOfMoves()) return  {-VALUE_WIN,emptyMove};


    vector<Move> possibleMoves = gm.getPossibleMoves();
    int score;
    int bestScore = -VALUE_WIN;
    Move sucMove = Move(0,0,0);
    Move bestMove = possibleMoves[0];

    if(alpha < boardValuation) alpha = boardValuation;
    if (! gm.board ->canJump(possibleMoves[0].pos,possibleMoves[0].rl,possibleMoves[0].ud)) {
        return {boardValuation, emptyMove};
    }

    for(auto move : possibleMoves){
        bool moved = gm.movePiece(move.pos,move.rl,move.ud);
        if(gm.getPlayerPlaying() == player){
            score = quiescence(gm, alpha, beta, gm.getPlayerPlaying()).first;
        }
        else{
            score = -quiescence(gm, -beta, -alpha, gm.getPlayerPlaying()).first;
        }
        if (moved)gm.undoMove();
        else{
            cout<< "error del movimiento " << move.str() << endl;
        }

        if(score > bestScore){
            bestScore = score;
            bestMove = move;
        }
        alpha = std::max(alpha, bestScore);

        if(alpha >= beta){
            break;
        }
    }
    return {bestScore,bestMove};
}
