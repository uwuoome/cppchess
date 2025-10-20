\#Chess AI Implementation in C++



\## Compiling to WASM

* Install EMSDK
* Set the environment variable using emsdk\\emsdk\_env.ps1 or .sh on Linux.
* Copy `minmax.cpp`, `chess.cpp` and `convert.cpp` and their headers into the build directory. 
* Copy `wasm.cpp` into the build directory (this will define the interface).
* Compile with target running in browser: `emcc wasm.cpp minmax.cpp chess.cpp convert.cpp -o index.html -s WASM=1 -lembind -s MODULARIZE=1 -s EXPORT_ES6=1`
* Edit `index.html` and add the function below to the Module object. 
* run an http server. e.g `npx http-server . -o -p 5555`
* press F12 to open the console and check the output. 


```          
        onRuntimeInitialized: function() {
            const abMinMaxFunction = Module.abMinMax; 
            const boardData = [
                ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k',
                ' ', ' ', ' ', ' ', 'R', ' ', ' ', ' ',
                ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                ' ', 'q', ' ', ' ', ' ', ' ', 'Q', ' ',
                ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                ' ', ' ', ' ', 'r', ' ', ' ', ' ', ' ',
                'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ];
            const Board = Module.Board;
            const cppBoard = new Board();
            for (let i = 0; i < boardData.length; i++) {
                cppBoard.push_back(boardData[i].charCodeAt(0)); 
            }
            const result = abMinMaxFunction(true, cppBoard, 4);
            cppBoard.delete();
            console.log(boardData.join(" ").match(/.{1,16}/g).join("\n"));  
            console.log("AI Move", result);
          }
```

