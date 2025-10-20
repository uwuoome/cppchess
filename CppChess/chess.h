#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <array>
#include <utility> 
#include <cstddef> 
#include <algorithm>

using namespace std;

enum CheckState { NotInCheck, Check, Checkmate, Stalemate };
struct PiecePotential {
	size_t from;
	vector<size_t> to;
};

// AI utilitiey fetches all pieces for weighing board state
unordered_map<char, vector<size_t>> piecesOnBoard(const array<char, 64>& board);
// gets all moves available to a player, used by AI
vector<PiecePotential> getPlayerMovesAvailable(bool isBlack, const array<char, 64>& board);
// gets all move targets that the piece at the from location can move to, used when player selects a piece
vector<size_t> validMoveTargets(size_t from, const array<char, 64>& board, bool flipped, int castling);
// gets the game state, used by AI or after move has been made to provide user feedback
CheckState getCheckState(bool irBlack, const array<char, 64>& board, bool flipped, int movesAvailable);
CheckState getCheckState(bool irBlack, const array<char, 64>& board, bool flipped);

// exposed for tests only
vector<size_t> kingMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> queenMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> rookMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> bishopMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> knightMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> pawnMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);