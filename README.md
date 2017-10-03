water_logic
===========

Solution for pouring water for web, cross-compiled to JS from C++ with Emscripten

[http://vfonic.github.io/water_logic/](http://vfonic.github.io/water_logic/)

This is the command to run to cross-compile cpp to javascript:

`emcc water.cpp -o water.html -s EXPORTED_FUNCTIONS="['_solve']" -s NO_EXIT_RUNTIME=1`

Then replace `water.js` from project.

ok
