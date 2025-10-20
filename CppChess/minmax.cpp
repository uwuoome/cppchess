#include "minmax.h"



// table values measured in centipawns, or 100th of a pawn's value, from black's perspective
array<char, 64> pawnTable = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5,  10, 10, -20,-20,10, 10, 5,
    5,  -5, -10,0,  0,  -10,-5, 5,
    0,  0,  0,  20, 20, 0,  0,  0,
    5,  5,  10, 25, 25, 10, 5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0,  0,  0,  0,  0,  0,  0,  0
};
array<char, 64> knightTable = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50
};
array<char, 64> bishopTable = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};
array<char, 64> rookTable = {
    0, 0, 5, 10, 10, 5, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    5, 10, 10, 10, 10, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0
};
array<char, 64> queenTable = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 5, 5, 5, 5, 5, 0, -10,
    0, 0, 5, 5, 5, 5, 0, -5,
    -5, 0, 5, 5, 5, 5, 0, -5,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20
};
array<char, 64> kingTable = {
    20, 30, 10, 0, 0, 10, 30, 20,
    20, 20, 0, 0, 0, 0, 20, 20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30
};

array<char, 64> getTable(char piece) {
    const char inUpperCase = piece & ~32;
    switch (inUpperCase) {
        case 'P': return pawnTable;
        case 'N': return knightTable;
        case 'B': return bishopTable;
        case 'R': return rookTable;
        case 'Q': return queenTable;
        default: return pawnTable;
    }
}

CheckState getCheckState(bool isBlack, array<char, 64> board) {
    return NotInCheck; // todo
}

char squareMirror(size_t index) { // reverses row on mirrored 64 element boards
    return index ^ 56;
}

int locationValue(char piece, const unordered_map<char, vector<size_t>>& pieces) {
    int result = 0;
    char black = piece | 32;
    char white = piece & ~32;
    array<char, 64> table = getTable(piece);
    const std::vector<size_t>& blackPositions = pieces.at(black);
    for (size_t i = 0; i < blackPositions.size(); i++) {
        int index = static_cast<int>(blackPositions[i]);
        result += table[index];
    }
    const std::vector<size_t>& whitePositions = pieces.at(white);
    for (size_t i = 0; i < whitePositions.size(); i++) {
        int index = static_cast<int>(squareMirror( whitePositions[i] ));
        result -= table[index];
    }
    return result;
}

int weighBoard(bool isBlack, const array<char, 64>& board, bool movesAvailable) {
    if (!movesAvailable) {
        CheckState checkState = getCheckState(isBlack, board);
        if (checkState == Checkmate) return isBlack ? -9999 : 9999;
        if (checkState == Stalemate) return 0;
        // this shouldn't occur
        return -35505;
    }
    unordered_map<char, vector<size_t>> pieces = piecesOnBoard(board);

    size_t pawns = (pieces['p'].size() - pieces['P'].size()) * 100;
    size_t knights = (pieces['k'].size() - pieces['K'].size()) * 320;
    size_t bishops = (pieces['b'].size() - pieces['B'].size()) * 330;
    size_t rooks = (pieces['r'].size() - pieces['R'].size()) * 500;
    size_t queens = (pieces['q'].size() - pieces['Q'].size()) * 900;
    int material = static_cast<int>(pawns + knights + bishops + rooks + queens);
    
    int ploc = locationValue('p', pieces);
    int nloc = locationValue('n', pieces);
    int bloc = locationValue('b', pieces);
    int rloc = locationValue('r', pieces);
    int qloc = locationValue('q', pieces);
    int locations =  ploc + nloc + bloc + rloc + qloc;
    
    return material +locations;
}

struct Move {
    size_t from;
    size_t to;
};

static int quiesce(int alpha, int beta, bool isBlack, const array<char, 64>& board) {
    vector<PiecePotential> moves = getPlayerMovesAvailable(isBlack, board);
    int boardValue = weighBoard(isBlack, board, moves.size() > 0);
    if (boardValue >= beta) {
        return beta;
    }
    if (alpha < boardValue) {
        alpha = boardValue;
    }
    auto isCapture = [&](size_t i) -> bool {
        string pieces = isBlack ? "pnbrqk" : "PNBRQK";
        return pieces.find((char)board[i]) != string::npos;
    };
    for (size_t i = 0; i < moves.size(); i++) {
        size_t from = moves[i].from;
        for (size_t t = 0; t < moves[i].to.size(); t++) {
            size_t to = moves[i].to[t];
            if (isCapture(to)) {
                array<char, 64> nextBoard = board;
                nextBoard[to] = board[from];
                nextBoard[from] = ' ';
                int score = -quiesce(-beta, -alpha, !isBlack, nextBoard);
                if (score >= beta) {
                    return beta;
                }
                if (score > alpha) {
                    alpha = score;
                }
            }
        }
    }
    return alpha;
}

static int alphaBeta(int alpha, int beta, bool isBlack, const array<char, 64>& board, int depth) {
    if (depth == 0) {
        return quiesce(alpha, beta, isBlack, board);
    }
    int bestScore = -99998;
    vector<PiecePotential> moves = getPlayerMovesAvailable(isBlack, board);
    if (moves.size() == 0) {
        CheckState cs = getCheckState(isBlack, board, false, 0);
        if (cs == Checkmate) {
            return isBlack ? 9999 : -9999;
        }
        return 0;
    }
    // TODO: check for insufficient material
    for (size_t i = 0; i < moves.size(); i++) {
        size_t from = moves[i].from;
        for (size_t t = 0; t < moves[i].to.size(); t++) {
            size_t to = moves[i].to[t];
            array<char, 64> nextBoard = board;
            nextBoard[to] = board[from];
            nextBoard[from] = ' ';
            int score = -alphaBeta(-beta, -alpha, !isBlack, nextBoard, depth - 1);
            if (score >= beta) {
                return score;
            }
            if (score > bestScore) {
                bestScore = score;
            }
            if (score > alpha) {
                alpha = score;
            }
        }
    }
    return bestScore;
}



string alphaBetaSearch(bool isBlack, const array<char, 64>& board, int depth) {

    Move bestMove = { 0, 0 };
    int bestValue = -99999;
    int alpha = -100000;
    int beta = 100000;
    vector<PiecePotential> moves = getPlayerMovesAvailable(isBlack, board);
    if (moves.size() == 0) return "";                               // game is a stalemate or checkmate  

    for (size_t i = 0; i < moves.size(); i++) {                     // for each piece that can be moved
        PiecePotential mp = moves[i];
        for (size_t t = 0; t < mp.to.size(); t++) {                 // for each tile that it can move to
            array<char, 64> nextBoard = board;  
            nextBoard[mp.from] = ' ';
            nextBoard[mp.to[t]] = board[mp.from];
            int boardValue = -alphaBeta(-beta, -alpha, !isBlack, nextBoard, depth - 1);
            if (boardValue > bestValue) {
                bestValue = boardValue;
                bestMove = { mp.from, mp.to[t] };
            }
            if (boardValue > alpha) {
                alpha = boardValue;
            }
        }
    }

    return toAlgebraic(bestMove.from, bestMove.to);
}