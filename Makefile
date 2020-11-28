run: compil
	./dico pg31469.txt

compil: text_to_tab.o principal.o
	gcc -o dico text_to_tab.o principal.o

text_to_tab.o:
	gcc -c text_to_tab.c

principal.o:
	gcc -c principal.c

clean:
	rm -f *.o
	rm -f dico
