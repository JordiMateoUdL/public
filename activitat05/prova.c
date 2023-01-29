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

#define READ_END 0  /* index pipe lectura */
#define WRITE_END 1 /* index pipe escritura */
#define EXIT_FAILURE 1
#define FIFO_NAME "fifoa"

int tiempoTerminado = 0;

void alarma();

int main(int argc, char *argv[])
{
    // control d'entrada de dades
    if (argc != 4)
    {
        printf("Error del numero de dades introduides.\nFORMA ADECUADA-> ./nombreEjecutable numeroJugadores tiempoSegundos clave\n");
        return 0;
    }

    int n = atoi(argv[1]), t = atoi(argv[2]); // n numero de jugadors , t segons de la partida.
    char *c;                                  // clave
    c = malloc(sizeof(char) * (strlen(argv[3]) + 1));
    strcpy(c, argv[3]);

    // crear FIFO

    int fd;

    remove(FIFO_NAME);

    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("Error fifo");
        return EXIT_FAILURE;
    }

    // Iniciar pipes per la comunicació amb cada fill. fdPipe[][x][]; x=0(pare), x=1(fill)
    /* int fdPipe[n][2][2];
     for (int i = 0; i < n; i++)
     {
         if (pipe(fdPipe[i][0][2]) == -1)
         {
             perror("pipe");
             exit(EXIT_FAILURE);
         }
         if (pipe(fdPipe[i][1][2]) == -1)
         {
             perror("pipe");
             exit(EXIT_FAILURE);
         }
     }*/

    pid_t pid[n];

    char *prova;
    prueba = malloc(sizeof(char) * 5);

    // execució óptima de fork()
    for (int i = 0; i < n; i++)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("Error fork");
            return EXIT_FAILURE;
        }

        if (pid[i] == 0)
        {
            prova = "./pruebaHijoMain";
            if (execl(prova, "", NULL) == -1)
            {
                perror("Error execl.");
                return EXIT_FAILURE;
            }
        }
    }

    free(prova);

    // Pare
    // acaba el programa al acabar tots els fills
    // sleep(2); // temps perque pare cree els fills? mirar si hi alguna manera -> fer servir SIGUSR2???

    signal(SIGALRM, alarma);
    printf("Inici del Room escape[%d] (%d segons)\n", getpid(), t);
    printf("--- Entren %d jugadors a la sala.\n", n);
    printf("--Clau: %s\n", c);
    alarm(t);

    fd = open(FIFO_NAME, O_RDONLY);
    char dato[50];
    int llegits = 0, usuariosAcertados = 0;
    while (!tiempoTerminado && usuariosAcertados < n)
    {
        if (llegits = read(fd, dato, sizeof(dato)) > 0)
        {
            if (strcmp(dato, c) == 0)
            {
                signal(SIGALRM, SIG_IGN);
                for (int i = 0; i < n; i++)
                    kill(pid[i], SIGUSR2);
                usuariosAcertados++;
            }
        }
    }
    close(fd);

    if (tiempoTerminado)
        printf("****DING DONG\n");

    else
        printf("Els jugadores obren el candau amb la clau: %s\n", dato);

    // Enviament de la senyal SIGUSR1 per la terminacio de procesos fills amb exit(1).
    for (int i = 0; i < n; i++)
        kill(pid[i], SIGUSR1);

    int status = 0;
    int identidadhijo;
    for (int i = 0; i < n; i++)
    {
        identidadhijo = wait(&status);
        // printf("\nPadre: El proces fill %d finalitzat amb estat %d\n", identidadhijo, status);
        printf("El jugador [%d] ha %s y surt de EscapeRoom [%d]\n", identidadhijo, usuariosAcertados == n ? "escapat" : "quedat atrapat", getpid());
    }

    if (tiempoTerminado)
    printf("els jugadors han perdut la partida, fins la seguent.\n");

    else
    printf("Els jugadors han guanyat la partida.\n");
    

    return 0;
}

void alarma()
{
    tiempoTerminado = 1;
}