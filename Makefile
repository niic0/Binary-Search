objets = principal.o avl_fct.o text_to_tab.o affiche_avl.o avl_search.o chrono.o

run: compil
	./dico the_shunned_house.txt

compil: $(objets)
	gcc -o dico $(objets)

profile: compil
	rm -rf callgrind*
	valgrind valgrind --tool=callgrind ./dico the_shunned_house.txt -f temps.txt
	kcachegrind callgrind*

%.o: %.c
	gcc -o $@ -c $< -Wall -g

clean:
	rm *.o
	rm dico
