build:
	gcc -std=c17 ../src/*.c -Wall -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -fno-common -Wformat-overflow -g -g3 -ggdb3 -I../lib/*.h -L../lib -o main.out `sdl2-config --cflags --libs` -lSDL2_image -lpthread 

valgrind:
	valgrind --leak-check=full -s --track-origins=yes --log-file=valgrind-out.txt ./main.out  

etc:
	--show-leak-kinds=all 
