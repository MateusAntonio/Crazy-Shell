trab: trab.o auxiliar.o
	gcc trab.o auxiliar.o -o trab

trab.o: trab.c
	gcc -c trab.c

auxiliar.o: auxiliar.c auxiliar.h
	gcc -c auxiliar.c

clean:
	rm *.o