objets = principal.o avl_fct.o text_to_tab.o avl_search.o chrono.o

run: compil
	./dico the_shunned_house.txt century ebooks

compil: $(objets)
	gcc -o dico $(objets)

%.o: %.c
	gcc -o $@ -c $< -Wall -g

clean:
	rm *.o
	rm dico
	clear
