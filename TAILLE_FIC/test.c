#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main (int argc, char *argv[]) {

	if (argc != 2) exit(1);
	struct stat buf;
	stat(argv[1], &buf);
	printf("## METHODE 1 -- stat\n");
	printf("Taille en octet : %lld\n",buf.st_size);
	printf("Documentation : man 2 stat\n\n");

	FILE *F;
	long long la_taille;
	F = fopen(argv[1],"r");
	if(F) {
		// On se place à la fin du fichier
		fseek (F, 0, SEEK_END);
		// On récupère le décalage par rapport au début
		la_taille = ftell (F);
		fclose (F);          
		printf("## METHODE 2 -- fseek ftell\n");
		printf("Taille en octet : %lld\n",la_taille);
		printf("Documentation : man fseek OU man ftell\n");
		
	}
	return 0;
}
