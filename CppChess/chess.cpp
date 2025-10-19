#include "chess.h"
#include <iostream>

unordered_map<char, vector<size_t>> piecesOnBoard(const array<char, 64>& board) {
    unordered_map<char, vector<size_t>> pieces = {
        {'P', {}}, {'N', {}}, {'B', {}}, {'R', {}}, {'Q', {}}, {'K', {}},
        {'p', {}}, {'n', {}}, {'b', {}}, {'r', {}}, {'q', {}}, {'k', {}}
    };
    for (size_t i = 0; i < 64; i++) {
        char piece = board[i];
        if (piece != ' ' && pieces.count(piece)) {
            pieces[piece].push_back(i);
        }
    }
    return pieces;
}

vector<size_t> getPlayerPieceIndices(bool isBlack, const array<char, 64>& board) {
    string pieceCodes = isBlack ? "prnbkq" : "PRNBKQ";
    vector<size_t> pieceIndices;
    for (size_t i = 0; i < 64; i++) {
        if (pieceCodes.find(board[i]) != string::npos) {
            pieceIndices.push_back(i);
        }
    }
    return pieceIndices;
}

static int getKingIndex(bool irBlack, const array<char, 64>& board) {
    char targetKing = irBlack ? 'k' : 'K';
    size_t kingIndex = distance(board.begin(), find(board.begin(), board.end(), targetKing));
    if (kingIndex == 64) return -1;
    return kingIndex;
}

static bool onBoard(size_t r, size_t c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

static bool pieceIsBlack(char piece) {
    return (piece & ~32) != piece;
}

static size_t homeRow(bool irBlack, bool flipped) {
    return irBlack == flipped ? 7 : 0;
}

static bool myPiece(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped) {
    size_t index = row * 8 + col;
    if (board[index] == ' ') return false;
    if (flipped) return irBlack != pieceIsBlack(board[index]);
    return irBlack == pieceIsBlack(board[index]);
}

static bool opPiece(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped) {
    size_t index = row * 8 + col;
    if (board[index] == ' ') return false;
    if (flipped) return irBlack == pieceIsBlack(board[index]);
    return irBlack != pieceIsBlack(board[index]);
}

static vector<size_t> project(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped, size_t dr, size_t dc){
    vector<size_t> result;
    size_t r = row + dr;
    size_t c = col + dc;
    while (onBoard(r, c)) {
        size_t idx = r * 8 + c;
        if (board[idx] == ' ') {
            result.push_back(idx);
        } else if (opPiece(irBlack, r, c, board, flipped)) {
           result.push_back(idx);
           return result; // stop after capturing opponent's piece
        } else if (myPiece(irBlack, r, c, board, flipped)) {
           return result; // blocked by own piece
        } else {
            return result; // TODO: handle error
        }
        r += dr;
        c += dc;
    }
    return result;
}

vector<size_t> kingMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped){
    auto validate = [&](size_t rowTo, size_t colTo) -> bool {
        return !myPiece(irBlack, rowTo, colTo, board, flipped); // target isnt an owned piece
    };
    vector<size_t> result;
    if (row > 0) {
        if (col > 0 && validate(row - 1, col - 1)) result.push_back((row-1)*8+(col-1));
        if (validate(row - 1, col)) result.push_back((row - 1)*8+col);
        if (col < 7 && validate(row - 1, col + 1)) result.push_back((row - 1)*8+(col + 1));
    }
    if (col > 0 && validate(row, col - 1)) result.push_back((row*8)+(col - 1));
    if (col < 7 && validate(row, col + 1)) result.push_back((row*8)+(col + 1));
    if (row < 7) {
        if (col > 0 && validate(row + 1, col - 1)) result.push_back((row + 1)*8+(col - 1));
        if (validate(row + 1, col)) result.push_back((row + 1)*8 +col);
        if (col < 7 && validate(row + 1, col + 1)) result.push_back((row + 1)*8+ (col + 1));
    }
    return result;
}

vector<size_t> queenMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped) {
    vector<size_t> result;
    const vector<std::pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},       
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}     
    };
    auto projectDir = [&](int dRow, int dCol) {
        return project(irBlack, row, col, board, flipped, dRow, dCol);
    };
    for (const auto& dir : directions) {
        vector<size_t> part = projectDir(dir.first, dir.second);
        result.insert(result.end(), part.begin(), part.end());
    }
    return result;
}

vector<size_t> rookMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped) {
    vector<size_t> result;
    const vector<pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    auto projectDir = [&](int dRow, int dCol) {
        return project(irBlack, row, col, board, flipped, dRow, dCol);
    };
    for (const auto& dir : directions) {
        vector<size_t> part = projectDir(dir.first, dir.second);
        result.insert(result.end(), part.begin(), part.end());
    }
    return result;
}

vector<size_t> bishopMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped) {
    vector<size_t> result;
    const vector<pair<int, int>> directions = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    auto projectDir = [&](int dRow, int dCol) {
        return project(irBlack, row, col, board, flipped, dRow, dCol);
    };
    for (const auto& dir : directions) {
        vector<size_t> part = projectDir(dir.first, dir.second);
        result.insert(result.end(), part.begin(), part.end());
    }
    return result;
}

vector<size_t> knightMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped) {
    vector<size_t> result;
    auto validMove = [&](pair<size_t, size_t>rc) {
        return onBoard(rc.first, rc.second) && (!myPiece(irBlack, rc.first, rc.second, board, flipped));
    };
    const vector<pair<size_t, size_t>> possibilities = {
        {row - 1, col + 2}, 
        {row - 1, col - 2},
        {row + 1, col + 2},
        {row + 1, col - 2},
        {row - 2, col + 1},
        {row - 2, col - 1},
        {row + 2, col + 1},
        {row + 2, col - 1}
    };
    for (size_t i = 0; i < possibilities.size(); i++) {
        if (validMove(possibilities[i])) {
            result.push_back(possibilities[i].first*8+ possibilities[i].second);
        }
    }
    return result;
}

vector<size_t> pawnMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped) {
    vector<size_t> result;
    auto scanAhead = [&](size_t nextRow) {
        if (board[nextRow * 8 + col] == ' ') result.push_back(nextRow * 8 + col);
        if (col > 0 && opPiece(irBlack, nextRow, col - 1, board, flipped)) result.push_back(nextRow * 8 + col - 1);
        if (col < 7 && opPiece(irBlack, nextRow, col + 1, board, flipped)) result.push_back(nextRow * 8 + col + 1);
    };
    if (irBlack) {
        //if(row < 7){ // TODO: at end of board turns to queen, so this check can then be removed
        if (row < 7) scanAhead(row + 1);
        if (row == 1 && board[16 + col] == ' ' && board[24 + col] == ' ') result.push_back(24 + col);
    }
    else {  //irWhite
        if (row > 0) scanAhead(row - 1);
        if (row == 6 && board[40 + col] == ' ' && board[32 + col] == ' ') result.push_back(32 + col);
    }
    return result;
}
static vector<size_t> _validMoveTargets(char piece, size_t index, const array<char, 64>& board, bool flipped) {
    size_t row = index / 8;
    size_t col = index % 8;
    char upperCasePieceCode = piece & ~32;
    bool isBlack = upperCasePieceCode != piece;
    switch (upperCasePieceCode) {
        case 'K': return kingMoves(isBlack, row, col, board, flipped);
        case 'Q': return queenMoves(isBlack, row, col, board, flipped);
        case 'R': return rookMoves(isBlack, row, col, board, flipped);
        case 'B': return bishopMoves(isBlack, row, col, board, flipped);
        case 'N': return knightMoves(isBlack, row, col, board, flipped);
        case 'P': return pawnMoves(isBlack, row, col, board, flipped);
        default: return {};
    }
}
static int pieceCanCapture(bool irBlack, const array<char, 64>& board, bool flipped, size_t captureIndex) {
    for (size_t i = 0; i < 64; i++) {
        if (board[i] == ' ') continue;
        bool isOpponentsPiece = (board[i] & ~32) != irBlack;
        if (!isOpponentsPiece) continue;
        vector<size_t> opponentMoves = _validMoveTargets(board[i], i, board, flipped);
        if (find(opponentMoves.begin(), opponentMoves.end(), captureIndex) != opponentMoves.end()) {
            return i;
        }
    }
    return -1;
}

vector<size_t> validMoveTargets(size_t from, const array<char, 64>& board, bool flipped, int castling) { 
    if (from < 0 || from > 63) {
        return {};
    }
    char pieceCode = board[from];
    vector<size_t> movesIgnoringCheck = _validMoveTargets(pieceCode, from, board, flipped);

    // only select moves that aren't entering check
    bool irBlack = pieceIsBlack(pieceCode);
    array<char, 64> permutableBoard = board;
    auto notMovingIntoCheck = [&](size_t moveIndex) -> bool {
        permutableBoard[from] = ' ';
        permutableBoard[moveIndex] = pieceCode;
        int kingIndex = getKingIndex(irBlack, permutableBoard);
        if (kingIndex == -1) return false;
        size_t checkFrom = pieceCanCapture(irBlack, permutableBoard, flipped, kingIndex);
        permutableBoard[from] = board[from];
        permutableBoard[moveIndex] = board[moveIndex];
        return checkFrom == -1;
    };
    vector<size_t> result;
    copy_if(
        movesIgnoringCheck.begin(), movesIgnoringCheck.end(), 
        back_inserter(result), notMovingIntoCheck
    );

    // if castling is available here add moves to result.
    bool notMovingKing = pieceCode != (irBlack ? 'k' : 'K');
    if (notMovingKing || castling == 0) {
        return result;
    }
    size_t home = homeRow(irBlack, flipped) * 8;
    auto movingThroughCheck = [&](size_t lcol, size_t rcol) -> bool {
        if (pieceCanCapture(irBlack, board, flipped, home + lcol) != -1) return true;
        if (pieceCanCapture(irBlack, board, flipped, home + rcol) != -1) return true;
        // additional tests for pawn control over the empty squares
        size_t pr = (home == 0 ? 1 : 6) * 8; 
        char opPawn = irBlack ? 'P' : 'p';
        if (board[pr + lcol - 1] == opPawn || board[pr + lcol] == opPawn) return true;
        if (board[pr + rcol] == opPawn || board[pr + rcol + 1] == opPawn) return true;
        return false;
    };
    if (castling & 1) {                                 // neither king nor left rook has moved yet
        bool lhsClear = (board[home + 1] == ' ' && board[home + 2] == ' ' && board[home + 3] == ' ');
        if (lhsClear && !movingThroughCheck(2, 3)) {
            result.push_back(home + 2);
        }
    }
    if (castling & 2) {                                 // neither king nor right rook has moved yet
        bool rhsClear = (board[home + 5] == ' ' && board[home + 6] == ' ');
        if (rhsClear && !movingThroughCheck(5, 6)) {
            result.push_back(home + 6);
        }
    }
    return result;
}

vector<PiecePotential> getPlayerMovesAvailable(bool isBlack, const array<char, 64>& board) {
    vector<size_t> fromList = getPlayerPieceIndices(isBlack, board);
    vector<PiecePotential> result;
    for (size_t i = 0; i < fromList.size(); i++) {                      // find all targets to move too
        size_t from = fromList[i];
        vector<size_t> to = validMoveTargets(from, board, false, 0);
        if (! to.empty()) {                                             // if 1 or more targets at to list
            PiecePotential movablePiece = {from, to};
            result.push_back(movablePiece);
        }
    }
    return result;
}

// used to check for stalemate
static bool playerHasAnyMovesAvailable(bool isBlack, const array<char, 64>& board) {
    vector<size_t> pieceIndices = getPlayerPieceIndices(isBlack, board);
    for (size_t i = 0; i < pieceIndices.size(); i++) {
        if (validMoveTargets(pieceIndices[i], board, false, 0).size() > 0) {
            return true;
        }
    }
    return false;
}

bool hasElement(vector<size_t> haystack, size_t needle) {
    return find(haystack.begin(), haystack.end(), needle) != haystack.end();
}

static vector<size_t> allPiecesCanCapture(size_t irBlack, const array<char, 64>& board, bool flipped, size_t captureIndex) {
    vector<size_t> result;
    for (size_t i = 0; i < 64; i++) {
        char piece = board[i];
        if (piece == ' ') continue;
        if (pieceIsBlack(piece) == irBlack) continue;
        vector<size_t> moves = _validMoveTargets(irBlack, i, board, flipped);
        if (hasElement(moves, captureIndex)) {
            result.push_back(i);
        }
    }
    return result;
}

static vector<size_t> openAdjacent(bool irBlack, size_t from, const array<char, 64>& board) {
    if (from < 0 || from > 63) return {};
    vector<size_t> result;
    size_t row = from / 8, col = from % 8;
    auto valid = [&](char piece) -> bool {
        if (piece == ' ') return true;
        return pieceIsBlack(piece) != irBlack;
    };
    if (row > 0) { // scan above
        if (col > 0 && valid(board[from - 9])) result.push_back(from - 9);
        if (valid(board[from - 8])) result.push_back(from - 8);
        if (col < 7 && valid(board[from - 7])) result.push_back(from - 7);
    }
    if (col > 0 && valid(board[from - 1])) result.push_back(from - 1);
    if (col < 7 && valid(board[from + 1])) result.push_back(from + 1);
    if (row < 7) { // scan below
        if (col > 0 && valid(board[from + 7])) result.push_back(from + 7);
        if (valid(board[from + 8])) result.push_back(from + 8);
        if (col < 7 && valid(board[from + 9])) result.push_back(from + 9);
    }
    return result;
}

CheckState getCheckState(bool irBlack, const array<char, 64>& board, bool flipped, int movesAvailable) {
    auto const isInStaleMate = [&]() -> bool {
        if (movesAvailable != -1) return movesAvailable;
        return !playerHasAnyMovesAvailable(irBlack, board);
    };
    int kingIndex = getKingIndex(irBlack, board);
    if (kingIndex == -1) {
        return Stalemate;   // should never happen
    }
    int checkFrom = pieceCanCapture(irBlack, board, flipped, kingIndex);
    if (checkFrom == -1) {
        if (isInStaleMate()) {
            return Stalemate;
        } else {
            return NotInCheck;
        }
    }
    // in check, now see if we can get out if it
    vector<size_t> checkRemovedBy = allPiecesCanCapture(!irBlack, board, flipped, checkFrom);
    bool canTakePieceToRemoveCheck = false;
    array<char, 64> permutableBoard = board;
    char kingPiece = irBlack ? 'k' : 'K';
    for (const size_t& removedByIndex : checkRemovedBy) {
        size_t nextKingIndex = board[removedByIndex] == kingPiece ? checkFrom : kingIndex;
        permutableBoard[removedByIndex] = ' ';
        permutableBoard[checkFrom] = board[removedByIndex];
        int secondCheckFrom = pieceCanCapture(irBlack, permutableBoard, flipped, nextKingIndex);
        if (secondCheckFrom == -1) {
            return Check;
        }
        permutableBoard[removedByIndex] = board[removedByIndex];
        permutableBoard[checkFrom] = board[checkFrom];
    }
    // see if a piece can interesect check.
     
    // no piece can capture all checking pieces so see if we can move the king out of check 
    vector<size_t>adjacentTiles = openAdjacent(irBlack, kingIndex, board);
    if (adjacentTiles.size() == 0) return Checkmate;

    // now we need to check that there's an open tile available that isn't moving into check
    for (auto& adj : adjacentTiles) {
        permutableBoard[adj] = permutableBoard[kingIndex];
        permutableBoard[kingIndex] = ' ';
        if (pieceCanCapture(irBlack, permutableBoard, flipped, adj) == -1) {
            return Check;
        }
        permutableBoard[kingIndex] = board[kingIndex];
        permutableBoard[adj] = board[adj];
    }

    return Checkmate;
}

CheckState getCheckState(bool irBlack, const array<char, 64>& board, bool flipped) {
    return getCheckState(irBlack, board, flipped, -1);
}
