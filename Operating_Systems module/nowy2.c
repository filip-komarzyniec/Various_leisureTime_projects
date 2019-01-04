/* Program obsługujący sygnały INT i TERM
----------------------------------------------
Filip Komarzyniec, 2. rok Teleinformatyki, AGH
----------------------------------------------
*/
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

void sig_react(int);	// deklaracja funkcji
void (*react)(int) = &sig_react;	// deklaracja i inicjalizacja wskaźnika do funkcji

int main() {

    printf("ch2: To jest drugi program o nazwie 'nowy2.c'\nch2: Teraz się zawieszam na 30s\n");
    sigset (SIGINT,react);
    sigset(SIGTERM,react);
    sleep(20);

    return 0;
}

void sig_react(int code) {
    switch(code) {
        case (2):
            printf("\nch2: Otrzymałem sygnał Interrupt o kodzie %d, wychodzę\n",code);
	    exit(2);
        case (15):
            printf("\nch2: Otrzymałem sygnał Terminated o kodzie %d, wychodzę\n",code);
            exit(15);
    }
}
