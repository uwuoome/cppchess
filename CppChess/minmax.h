#pragma once
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <array>
#include <vector>
#include "chess.h"
#include "convert.h"

using namespace std;

array<char, 64> getTable(char piece);
int weighBoard(bool isBlack, const array<char, 64>& board, bool movesAvailable);
std::string findMove(/*char board[64]*/);
string alphaBetaSearch(bool isBlack, const array<char, 64>& board, int depth);
