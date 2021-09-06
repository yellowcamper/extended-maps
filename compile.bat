@echo off
C:/"Program Files"/doxygen/bin/doxygen.exe Doxyfile
start Doxygen/html/index.html
g++ -o debug.exe *.cpp
debug.exe
