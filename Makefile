all: 
	g++ Game.cpp main.cpp -I /opt/homebrew/include -o app -L /opt/homebrew/lib/ -lSDL2 -lSDL2_image