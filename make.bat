
cd bin
del Snake.exe
cd..
cl user32.lib src/*.cpp /I include /EHsc
move *.obj obj
move *.exe bin
"bin/Snake.exe"
