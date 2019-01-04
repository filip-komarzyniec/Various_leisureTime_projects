/* Program obsługujący sygnały INT i TERM
----------------------------------------------
Filip Komarzyniec, 2. rok Teleinformatyki, AGH
----------------------------------------------
*/
#include<stdio.h>
#include<unistd.h>	/*nagłówek do funkcji sleep*/
#include<signal.h>	/*nagłówek do obsługi sygnałów*/
#include<stdlib.h>

void sig_react(int);	// deklaracja funkcji

int main(){
    printf("ch1: To jest pierwszy program o nazwie 'nowy1.c'\nch1: Teraz się zawieszam na 20s\n");
    void(*react)(int) = &sig_react;	/* deklaracja i inicjalizacja wskaźnika*/
    sigset(SIGINT,react);
    sigset(SIGTERM,react);
    sleep(20);
return 0;
}
void sig_react(int code) {
    switch(code) {
        case (2):
            printf("\nch1: Otrzymałem sygnał Interrupt o kodzie %d, wychodzę\n",code);
            exit(2);
        case (15):
            printf("\nch1: Otrzymałem sygnał Terminated o kodzie %d, wychodzę\n",code);
            exit(15);
    }
}
