#include "convert.h"

string toAlgebraic(size_t index) {
	if (index < 0 || index > 63) return "N/A";
	size_t row = index / 8;
	size_t col = index % 8;
	char letter = (char)('a' + col);
	char number = (char)('1' + (7 - row));
	std::string result;
	result += letter;
	result += number;
	return result;
}

string toAlgebraic(size_t from, size_t to) {
	return toAlgebraic(from) + toAlgebraic(to);
}

string checkStateToString(CheckState state) {
    switch (state) {
		case NotInCheck:	return "Not in Check";
		case Check:			return "In Check";
		case Checkmate:		return "Checkmate";
		case Stalemate:		return "Stalemate";
		default:			return "Unknown State";
    }
}