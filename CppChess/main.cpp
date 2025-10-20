#include <iostream>
#include <chrono>
#include "minmax.h"
#include "console.h"

array<char, 64> testBoard = {
  'r', ' ', 'b', 'q', 'k', 'b', 'n', 'r',
  'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
  ' ', ' ', 'n', ' ', ' ', ' ', ' ', ' ',
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  ' ', ' ', ' ', 'P', 'P', ' ', ' ', ' ',
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  'P', 'P', 'P', ' ', ' ', 'P', 'P', 'P',
  'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
};

int main()
{
    array<char, 64> table = getTable('p');
    printBoard(testBoard);
    
    int depth = 4;
    auto start = chrono::high_resolution_clock::now();
    cout << alphaBetaSearch(true, testBoard, depth) << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time taken for depth " << depth << " search: " << duration.count() << " milliseconds" << endl;

    vector<PiecePotential> moves = getPlayerMovesAvailable(true, testBoard);
    printMovesAvailable(moves);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
