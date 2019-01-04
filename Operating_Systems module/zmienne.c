/* Program wypisujący zmienne środowiskowe
-----------------------------------------------
Filip Komarzyniec, 2. rok Teleinformatyki, AGH 
----------------------------------------------- */

#include <stdio.h>  
#include <stdlib.h>
#include <unistd.h>	/*nagłówek do funkcji sleep*/

int main (int argc, char *argv[], char *envp[]) {
	if (argc==1) {		// jeśli nie podano żadnych argumentów
		printf("nie podano argumentow, wypisuje wszystkie zmiennne środowiskowe\n");
		sleep(3);
		int i =1;
		while (*envp) {		// dopóki elementy w tablicy 
			printf("%d Zmienna: %s\n",i++, *envp++);	// wypisuję jako stringi po nowej linii 
		}	
	}
	else {
		argv++;
		for(;*argv;argv++) {
			char *var = getenv(*argv);
			if (var == NULL) {		/*jeśli nie istnieje taka zmienna środowiskowa*/
				printf("Zmienna %s: Nie ma takiej zmiennej środowiskowej\n",*argv);
			}			
			else
				printf("Zmienna %s: %s\n",*argv,var);
		}
}




return 0;
}
