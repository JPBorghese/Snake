cd bin
del Snake.exe
cd ..
cl /Fe"bin/Snake" user32.lib src/*.cpp /I include /EHsc
move *.obj obj
REM "bin/Snake.exe"