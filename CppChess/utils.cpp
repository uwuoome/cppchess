#include "utils.h"

void printBoard(const array<char, 64>& board) {
	cout << "  A B C D E F G H\n";
	cout << " +----------------+\n";
	for (int row = 0; row < 8; row++) {
		cout << (8-row) << "|";
		for (int col = 0; col < 8; col++) {
			cout << board[row * 8 + col] << " ";
		}
		cout << "|\n";
	}
	cout << " +----------------+\n";
}

void printPositionTable(const array<char, 64>& board) {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			std::cout << std::setw(3) << std::to_string(board[row * 8 + col]) << " ";
		}
		std::cout << "\n";
	}
}

void printMovesAvailable(const vector<PiecePotential>& moves) {
	cout << "Pieces to move available: " << moves.size() << "\n";
	for (size_t i = 0; i < moves.size(); i++) {
		cout << "Piece at r " << toAlgebraic(moves[i].from) << " can move to: ";
		const vector<size_t> to = moves[i].to;
		for (size_t m = 0; m < to.size(); m++) {
			cout << toAlgebraic(to[m]) << " ";
		}
		cout << "\n";
	}
}
