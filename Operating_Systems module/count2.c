/* Program wypisujący liczby nieparzyste na ekranie w rzędzie. Dzięki zastosowaniu kolejki komunikatów
synchronizuje się z programem 'count2.c'. Zaimplementowana jest także obsługa sygnału INT, po którego otrzymaniu 
program usuwa stworzoną kolejkę komunikatów. Dzięki temu 'count2.c' też konczy swoje dzialanie.
---------------------------------------------------------------------------------------------------------------
Filip Komarzyniec, 2. rok Teleinformatyki AGH
---------------------------------------------------------------------------------------------------------------
*/
#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/msg.h>	// nagłówek do kolejek komunikatów
#include<sys/ipc.h>	// tworzenie unikalnego klucza
#include<signal.h>      // do obsługi sygnału
// obsługa sygnału INT
void sigint();

// definiuję strukturę komunikatu
struct msgbuf{
	long mtype;
};
/* Tworząc w obu programach taki sam klucz i przy jego użyciu tworząc kolejkę komunikatów zapewniam, 
że nie stworzę dwóch kolejek. Jeśli wywołuję funkcję msgget() z kluczem już skojarzonym z kolejką komunikatów,
funkcja msgget() zwraca identyfikator istniejącej kolejki. Dzięki temu mogę włączać programy w dowolnej
kolejności, z dowolnym opóźnieniem, tak jak należało w zadaniu. */

int queID;
int main(){
        int i;
        int j;
        key_t key;
        int rcv;
//int queID; obsługa syngału INT
        void (*react)() = &sigint;      // deklaracja i definicja wskaźnika do funkcji
        sigset (SIGINT,react);
// tworzę taki sam klucz w obu programach
        key = ftok("count1",'Q');
        if (key > 0){;}
        else{perror("count2:nie udało się stworzyć klucza");}
// tworzę kolejkę komunikatów
        struct msgbuf *ptr, komunikat; ptr = &komunikat;
        queID = msgget(key,0660|IPC_CREAT);     // prawo zapisu i odczytu dla właściciela i grupy
        if (queID > 0){printf("count2: kolejka stworzona, ID %d\n",queID);}
        else{perror("count2: Nie udało się stworzyć kolejki");}
//
        for(i=2;i<200;i=i+2){
        // czekam na komunikat typu 2, czy można kontynuować
                if((rcv=msgrcv(queID,ptr,0,2,0))<0){_exit(2);}  // jeśli błąd to wychodzę
                ptr->mtype = 1;         // nadpisuję pole, bo po odebraniu komunikatu wartość wynosiła 2
        /*poczatek sekcji krytycznej */
                for(j=0;j<i;j++){
                        printf("%d ",i);
                        fflush(stdout);
                        sleep(1);
                }
                printf("\n");
        /*koniec sekcji krytycznej */
        // wysyłam komunikat typu 1, że pierwszy program może kontynuować
                int msg = msgsnd(queID,ptr,0,0);

        }
        return 0;
}
void sigint(){  // na sygnał INT kasuję kolejkę komunikatów
        struct msqid_ds *ptr2,kolejka;// wskaźnik do struktury stworzonej podczas wywołania msgget()
	ptr2 = &kolejka;
        int del = msgctl(queID,IPC_RMID,ptr2);
	if (del == 0){printf("\ncount2: usunałem pomyślnie kolejkę z systemu\n");}
        else {perror("count2: Nie skasowano kolejki z systemu");}
        _exit(2);       
}