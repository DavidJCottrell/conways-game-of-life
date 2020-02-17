CXXFLAGS = -Wall -F /Library/Frameworks
SDL = -framework SDL2 -F /Library/Frameworks -I /Library/Frameworks/SDL2.framework/Headers
SDL_IMAGE = -framework SDL2_image -F /Library/Frameworks -I /Library/Frameworks/SDL2_image.framework/Headers

all: build cleanrun

#Compile to machine code
build: main.o Game.o
	g++ main.o Game.o -o game $(SDL) $(SDL_IMAGE)

#Compile to object
obj/main.o : main.cpp Game.cpp
	g++ $(CXXFLAGS) -c main.cpp Game.cpp

clean:
	@echo Cleaning...
	@rm *.o game
	@echo Complete.
	#clear

run:
	@echo Running game...
	@./game

cleanrun: run clean