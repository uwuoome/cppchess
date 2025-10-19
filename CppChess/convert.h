#pragma once
#include <string>

#include "chess.h"

using namespace std;

string toAlgebraic(size_t index);
string toAlgebraic(size_t from, size_t to);
string checkStateToString(CheckState state);
