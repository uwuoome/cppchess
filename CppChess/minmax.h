#pragma once
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <array>
#include <vector>
#include <future>
#include <tuple>
#include <iostream>
#include "chess.h"
#include "convert.h"

using namespace std;

array<int, 64> getTable(char piece);
int weighBoard(bool isBlack, const array<char, 64>& board, bool movesAvailable);
string alphaBetaSearch(bool isBlack, const array<char, 64>& board, int depth);
