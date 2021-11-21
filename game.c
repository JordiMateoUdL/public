#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "helpers/helpers.h"
#include "pokedex/pokedex.h"

char *args[] = {"pokemon", "pokemon", NULL};
 
int main(int arc, char *arv[])
{

char s[100];
init_pokedex();
srand(getpid());

int endFlag=1;
while (endFlag == 1) {
int st;
char choice;
sprintf(s, "################\n# E. Explore \n# Q. Quit\n################\n");
if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
scanf(" %c", &choice);
 

switch (choice) {
    case 'Q':
    endFlag=0;
    break;
    case 'E':
    sprintf(s, "%s", KMAG, KNRM);
    if (write(1, s, strlen(s)) < 0) perror("Error writting colour KMAG");
    int pos = getRandom(151);
    show_pokemon(pos);
    sprintf(s, "%s", KNRM);
    if (write(1, s, strlen(s)) < 0) perror("Error writting colour KNRM");

    int pokemon_pid = fork();
    int endExplore=1;
    while (endExplore == 1){
        int validTurn = 1;
        if (pokemon_pid == 0){
            execv("./pokemon", args);
            perror("Error exec");
        }
        else{
            sprintf(s, "################\n# P. Throw pokeball \n# B. Throw berry \n# R. Run\n################\n");
            if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
            scanf(" %c", &choice);
            switch (choice) {
                case 'P':
                    kill(pokemon_pid, SIGUSR1);
                    break;
                case 'B':
                    kill(pokemon_pid, SIGUSR2);
                    break;
                case 'R':
                    kill(pokemon_pid, SIGINT);
                    break;
                default:
                    sprintf(s, "%s!!!!Invalid option. Try again. %s\n", KRED, KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting invalid option");
                    validTurn = 0;
            }

            if (validTurn == 1){
                waitpid(-1, &st, WUNTRACED);
                int status = WEXITSTATUS(st);
                if(status==PokemonEscaped){
                    sprintf(s, "%sThe pokemon escaped already%s\n", KYEL,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    endExplore=0;
                } else if (status==PokemonCaught) {
                    sprintf(s, "%sGotcha!The pokemon was caught.%s\n", KGRN,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    endExplore=0;
                } else if (status==AshEscaped) {
                    sprintf(s, "%sYou have escaped safely.%s\n", KRED,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    endExplore=0;
                } else{
                    sprintf(s, "%sOh no!The pokemon broke free.%s\n", KBLU,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    kill(pokemon_pid, SIGCONT);
                }
            }
        }
    }
    break;
    default:
        sprintf(s, "%s!!!!Invalid option. Try again. %s\n", KRED, KNRM);
        if (write(1, s, strlen(s)) < 0) perror("Error writting invalid option");
    }
}
 
sprintf(s, "%s!!!!I'm tired from all the fun... %s\n", KMAG, KNRM);
if (write(1, s, strlen(s)) < 0) perror("Error writting the ending msg");
exit(0);
 
}