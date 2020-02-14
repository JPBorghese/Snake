
cl /Fe"bin/Snake" user32.lib src/*.cpp /I include /EHsc
move *.obj obj
"bin/Snake.exe"