gcc:
	gcc -std=c17 ../src/*.c -Wall -Wshadow -Wdouble-promotion -Wformat=2 -Wformat-truncation -fno-common -Wformat-overflow -g -g3 -ggdb3 -I../lib/*.h -L../lib -o main.out `sdl2-config --cflags --libs` -lSDL2_image

clang:
	clang -std=c17 ../src/*.c -Wall -Wshadow -Wdouble-promotion -fno-common -g -g3 -ggdb3 -I../lib/*.h -L../lib -o main.out `sdl2-config --cflags --libs` -lSDL2_image

valgrind:
	valgrind --leak-check=full -s --track-origins=yes --log-file=valgrind-out.txt ./main.out  

etc:
	--show-leak-kinds=all 
