#include <iostream>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "minmax.h"


std::string abMinMax(bool isBlack, const std::vector<char>& vboard, int depth,
                      AbortSignal& signal, emscripten::val onStep) {
    if (vboard.size() != 64) {
        return "error: invalid board size."; 
    }
    std::array<char, 64> aboard;
    std::copy(vboard.begin(), vboard.end(), aboard.begin());
    std::string result = alphaBetaSearch(isBlack, aboard, depth, signal, onStep); 
    return result;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::register_vector<char>("Board");
    emscripten::class_<AbortSignal>("AbortSignal")
        .constructor<>()
        .property("aborted", &AbortSignal::aborted); // Allow JS to read/write 'aborted' property
    emscripten::function("abMinMax", 
        (std::string(*)(bool, const std::vector<char>&, int, AbortSignal&, emscripten::val)) &abMinMax
    );
}
int main() {
    std::cout << "Wasm AI module initialized." << std::endl;
    return 0;
}