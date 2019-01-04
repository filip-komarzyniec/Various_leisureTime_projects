#### Programmes written in C for passing _Operating Systems_ module  

* **zmienne.c**  
	This programme without any argumets passed prints all environmental variables. With arguments, it treats them as environmental variables and prints them. The programme yields a proper warning if arguments passed are not environmental variables.  
* **nowy1.c** and **nowy2.c**  
	These programmes print one line and sleep for, accordingly, 20 and 30 sec. They both have implemented handling of INT and TERM signals and in such case, their return code is > 0.
* **fkexe.c**
	The programme creates two new processes using fork() function and swaps their code for _nowy1.c_ and _nowy2.c_ programmes. Then it waits for the child proccesses to finish, gets their return codes, prints them and exits itself.  
