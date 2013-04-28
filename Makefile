exe: src/main.cpp src/Game.cpp src/utils.cpp
	g++ -c src/main.cpp
	g++ -c src/Game.cpp
	g++ -c src/utils.cpp
	g++ -o ld26 main.o Game.o utils.o -lGL -lGLU -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
clear:
	rm *.o
	rm debug.txt
	rm ld26
	