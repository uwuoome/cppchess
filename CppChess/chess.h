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

unordered_map<char, vector<char>> piecesOnBoard(const array<char, 64>& board);
vector<char> validMoveTargets(char from, const array<char, 64>& board, bool flipped, int castling);
vector<PiecePotential> getPlayerMovesAvailable(bool isBlack, const array<char, 64>& board);

// exposed for tests only
vector<size_t> kingMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> queenMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> rookMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> bishopMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> knightMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);
vector<size_t> pawnMoves(bool irBlack, size_t row, size_t col, const array<char, 64>& board, bool flipped);