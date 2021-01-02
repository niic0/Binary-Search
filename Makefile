objets = principal.o avl_fct.o text_to_tab.o avl_search.o affiche_avl.o

run: compil
	./dico test

compil: $(objets)
	gcc -o dico $(objets)

%.o: %.c
	gcc -o $@ -c $< -Wall

clean:
	rm *.o
	rm dico
	clear
