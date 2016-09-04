all:
	cd source; g++ MyGame.cpp -o sniper -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lGL -lGLU -lglut -lSDL -lSDL_image -lpthread 
	mv source/sniper ../sniperLeague
	./sniper

compile:
	cd source; g++ MyGame.cpp -o sniper -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lGL -lGLU -lglut -lSDL -lSDL_image -lpthread 
	mv source/sniper ../sniperLeague
	./sniper

debug:
	cd source; g++ MyGame.cpp -o dsniper -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lGL -lGLU -lglut -lSDL -lSDL_image -lpthread -Ddebugmain -Ddebugnet
	mv source/dsniper ../sniperLeague
	./dsniper

debugmain:
	cd source; g++ MyGame.cpp -o dmainsniper -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lGL -lGLU -lglut -lSDL -lSDL_image -lpthread -Ddebugmain
	mv source/dmainsniper ../sniperLeague
	./dmainsniper

debugnet:
	cd source; g++ MyGame.cpp -o dnetsniper -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lGL -lGLU -lglut -lSDL -lSDL_image -lpthread -Ddebugnet
	mv source/dnetsniper ../sniperLeague
	./dnetsniper

run:
	./sniper

view: doc
	evince Documentation/main.pdf

doc:
	cd Documentation/latex;	make 
	
readme:
	gedit source/ReadMe
	
clean:
	rm -rf ./*sniper
	cd Documentation/latex; make clean