#include <sys/time.h>
#include <stdlib.h>


double chrono(int action) {
	static double ch = 0.0;
	double current;
	struct timeval tv;
	
	gettimeofday(&tv,NULL);
	current = tv.tv_sec + (float)(tv.tv_usec)*1e-6;
	if (action) ch = current;
	return current-ch;
}

// Déclenchement du chrono
// Le remet à zéro s'il était déjà lancé
void  chrono_reset() {
	chrono(1);
}

// Renvoie la valeur courante du chrono en secondes
// La précision est la microseconde
double chrono_lap() {
	return chrono(0);
}
