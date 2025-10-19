#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <array>

#include "chess.h"
#include "convert.h"

using namespace std;

void printBoard(const array<char, 64>& board);
void printPositionTable(const array<char, 64>& board);
void printMovesAvailable(const vector<PiecePotential>& moves);
void printCheckState(bool asBlack, const array<char, 64>& board);