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

using ProgressCallback = std::function<void(int, int)>;
class AbortSignal {
public:
    bool aborted = false;
};

static AbortSignal defaultAbortSignal;// never aborted for searches without abort signals

array<int, 64> getTable(char piece);
int weighBoard(bool isBlack, const array<char, 64>& board, bool movesAvailable);
string alphaBetaSearch(bool isBlack, const array<char, 64>& board, int depth, AbortSignal& signal, ProgressCallback onStep);
string alphaBetaSearch(bool isBlack, const array<char, 64>& board, int depth);
