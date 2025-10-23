# Chess AI Implementation in C++



## Compiling to WASM

* Install EMSDK.
* Set the environment variable using `emsdk_env.ps1` (or .sh on Linux) from the emsdk install directory .
* Copy `minmax.cpp`, `chess.cpp` and `convert.cpp` and their headers from `./CppChess`into the build directory. 
* Copy `wasm.cpp` and `index.html` from `./wasm` into the build directory (wasm.cpp will define the interface).
* Compile WASM and JS loader bundled into a single file: 
`emcc wasm.cpp minmax.cpp chess.cpp convert.cpp -o wasm.js -s WASM=1 -lembind -s EXPORT_ES6=1 -s SINGLE_FILE=1 -O2`
* run an http server. e.g `npx http-server . -o -p 5555`
* Point your browser at the server and a result should appear.
* To deploy: copy `wasm.js` to deployment directory. Open index.html and copy the code that loads the module into your JavaScript. 
