
windres resource\resource.rc -o resource\resource.o
g++ src\*.cpp resource\resource.o FPlitter.cpp -Wall -mwindows -lcomctl32 -o exe\FPlitter++.exe
echo done compiling
pause