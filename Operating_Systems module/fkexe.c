/* Program tworzący dwa procesy potomne, odbierający od nich kod zakończenia
--------------------------------------------------------------------------
Filip Komarzyniec, 2. rok Teleinformatyki, AGH
--------------------------------------------------------------------------
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>

int kill(pid_t, int);	// deklaracja funkcji, zgodnie ze standardem 

int main() {
    pid_t pid1,pid2, pid;
    int status;
    pid1 = fork();
    if (pid1 == 0) {
    // child process
	printf("ch1: Mój ID =%d\n",getpid());
    	//execlp("/Users/filip/Desktop/prog1","prog1",NULL);
	execlp("prog1","prog1",NULL);
	// tutaj dochodzę tylko w przypadku błędu podmiany kodu
	perror("exec 1 failed");
	exit(101+getpid());
    }
    pid2 = fork();
    if (pid2 == 0) {
    // child process
	printf("ch2: Mój ID =%d\n",getpid());
	//execlp("/Users/filip/Desktop/a.out","a.out",NULL);
	execlp("a.out","a.out",NULL);
	//tutaj dochodzę tylko w przypadku błędu podmiany kodu
	perror("exec 2 failed");
	exit(102+getpid());
    }
    if (pid1 > 0 && pid2 > 0){
    // parent process
	printf("P: czekam na śmierć dzieci\n");
        while((pid=wait(&status)) > 0){
	    if (WIFEXITED(status)) {
	    // jeśli dziecko zakończyło się normalnie, tzn. wyszło z main albo funkcją exit() lub _exit()
	   	printf("P: Dziecko umarło dobrze\n");
	    	printf("P: Proces %d zakończył się kodem %d\n",pid,WEXITSTATUS(status));
	    ;}
	    else {printf("P: Śmierć była inna\n");}
        }
    	printf("P: Nie ma więcej procesów potomnych, kończę działanie\n");
    }
    else {
    // jeśli któryś fork się nie udał
	printf("fork failed, aborting\n");
	kill(0,SIGTERM);	// zabijam wszystkie procesy z grupy jako rodzic
    }
    return 0;
}
