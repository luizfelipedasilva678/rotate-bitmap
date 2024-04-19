rotate-bitmap: rotate-bitmap.o entry-point.o
	gcc -Wall -Werror -pedantic-errors -o ./build/rotate-bitmap ./obj/rotate-bitmap.o ./obj/entry-point.o

entry-point.o: ./src/entry-point.c
	gcc -Wall -Werror -pedantic-errors -c -o ./obj/entry-point.o ./src/entry-point.c

rotate-bitmap.o: ./src/lib/rotate-bitmap.c
	gcc -Wall -Werror -pedantic-errors -c -o  ./obj/rotate-bitmap.o ./src/lib/rotate-bitmap.c
	
clean:
	rm ./obj/rotate-bitmap.o ./obj/entry-point.o