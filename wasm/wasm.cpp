#include <iostream>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include "minmax.h"

std::string abMinMax(bool isBlack, const std::vector<char>& vboard, int depth) {
    if (vboard.size() != 64) {
        return "error: invalid board size."; 
    }
    std::array<char, 64> aboard;
    std::copy(vboard.begin(), vboard.end(), aboard.begin());
    std::string result = alphaBetaSearch(isBlack, aboard, depth); 
    return result;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::register_vector<char>("Board");
    emscripten::function("abMinMax", &abMinMax);
}
int main() {
    std::cout << "Wasm AI module initialized." << std::endl;
    return 0;
}