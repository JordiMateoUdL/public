#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define READ_END 0  /* index pipe lectura */
#define WRITE_END 1 /* index pipe escritura */
#define EXIT_FAILURE 1
#define FIFO_NAME "fifoa"

void funcioSenyals(int sig);
void paraulaAleatoria(char *paraula);

int acertat = 0;
int exit=0;

int main(int argc, char const *argv[])
{
    signal(SIGUSR1, funcioSenyals);
    signal(SIGUSR2, funcioSenyals);
    srand(time(NULL));

    int fd;

    printf("EscapeRoom [%d] ha entrat el jugador [%d]\n", getppid(), getpid());

    char paraula[50];
    // paraula = malloc(sizeof(char) * 5);

    fd = open(FIFO_NAME, O_WRONLY);

    
    while (!exito)
    {
        paraulaAleatoria(paraula);
        if (strlen(paraula) > 0)
            write(fd, paraula, sizeof(paraula));
    }
    close(fd);

    // if (exit)

    printf("El jugador [%d] ha %s y surt de EscapeRoom [%d]\n", getpid(), exito ? "escapat" : "quedat atrapat", getppid());

    exit(1);

    return 0;
}

void funcioSenyals(int sig)
{
    if (SIGUSR1 == sig)
        exit=1;
    else
        acertat = 1;
}

void paraulaAleatoria(char *paraula)
{
    strcpy(paraula, "");
    for (int i = 0; i < 4; i++)
        paraula[i] = 'a' + (rand() % (26));
        //paraula[i] = (rand() % 2 == 0 ? 'A' : 'a') + (rand() % (26));
    paraula[4] = '\0';
}