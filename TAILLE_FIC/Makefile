run: test zip
	./test pg31469.txt
	ls -l pg31469.txt

test: test.c
	gcc -Wall -o test test.c


clean:
	rm -f test
	ls -l

zip: taille_fic.zip

DIRNAME=TAILLE_FIC
taille_fic.zip: test.c Makefile
	rm -rf $(DIRNAME)
	mkdir $(DIRNAME)
	cp test.c Makefile pg31469.txt $(DIRNAME)
	zip -r taille_fic.zip $(DIRNAME)
	rm -rf $(DIRNAME)
	ls -l
	
