perso:main.o perso.o
	gcc main.o perso.o -o perso -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -g
main.o:main.c perso.h
	gcc -c main.c -g 
perso.o:perso.c perso.h
	gcc -c perso.c -g 

