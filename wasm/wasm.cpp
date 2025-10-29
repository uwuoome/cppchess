#include <iostream>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "minmax.h"


std::string abMinMax(bool isBlack, const std::vector<char>& vboard, int depth, emscripten::val onStep) {
    if (vboard.size() != 64) {
        return "error: invalid board size."; 
    }
    // specify that the callback returns bool abort value here
    ProgressCallback callback = [onStep](int d, int s) -> bool {
        return onStep(d, s).as<bool>();
    };
    std::array<char, 64> aboard;
    std::copy(vboard.begin(), vboard.end(), aboard.begin());
    std::string result = alphaBetaSearch(isBlack, aboard, depth, callback); 
    return result;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::register_vector<char>("Board");
    emscripten::function("abMinMax", 
        (std::string(*)(bool, const std::vector<char>&, int, emscripten::val)) &abMinMax
    );
}
int main() {
    std::cout << "Wasm AI module initialized." << std::endl;
    return 0;
}