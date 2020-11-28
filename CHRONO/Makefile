run: test zip
	./test

test: chrono.o test.o
	gcc -o test chrono.o test.o

chrono.o: chrono.h chrono.c
	gcc -c -Wall chrono.c

test.o: chrono.h test.c
	gcc -c -Wall test.c

clean:
	rm -f chrono.o
	rm -f test.o
	rm -f test
	ls -l

zip: chrono.zip

DIRNAME=CHRONO
chrono.zip: chrono.c chrono.h test.c Makefile
	rm -rf $(DIRNAME)
	mkdir $(DIRNAME)
	cp chrono.c chrono.h test.c Makefile $(DIRNAME)
	zip -r chrono.zip $(DIRNAME)
	rm -rf $(DIRNAME)
	ls -l
	
