#pragma once
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <array>
#include <vector>
#include "chess.h"

using namespace std;

array<char, 64> getTable(char piece);
int weighBoard(bool isBlack, const array<char, 64>& board, bool movesAvailable);
std::string findMove(/*char board[64]*/);
