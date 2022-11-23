#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>


#define READ_END    0    /* index pipe lectura */
#define WRITE_END   1    /* index pipe escritura */
#define EXIT_FAILURE 1

#define FILE_NAME  "user.txt"   /* nom del fitxer */

int main(int argc, char* argv[])   
{
    int fd1[2],fd2;
    int status, pid;
    
    pipe(fd1);                  /* es crea el pipe */
    
    pid = fork();     

    if(pid == 0)                /* fill 1 */
    {              
        close(fd1[READ_END]);   /* tanquem el fd1[0] */
        
        dup2(fd1[WRITE_END], STDOUT_FILENO); 
        close(fd1[WRITE_END]);
        
        execlp("/bin/grep", "grep", "$(whoami)", NULL);
    }
    else if (pid == -1){
        perror("Error fork");
        return EXIT_FAILURE;
    }
    else                          /* pare */
    { 
        close(fd1[WRITE_END]);    
        
        pid = fork();
		
       if(pid == 0)               /* fill 2 */
       {     
          fd2 = open(FILE_NAME, O_WRONLY);
          dup2(fd1[READ_END], STDIN_FILENO);
          close(fd1[READ_END]);
		   		   
          dup2(fd2, STDOUT_FILENO);		   
            
          execlp("/usr/etc/passwd", NULL);
       }
       else if (pid == -1){
        perror("Error fork");
        return EXIT_FAILURE;
    }
       else /* pare */
       {
            close(fd1[READ_END]);    
       }                
     }
    
   /* fem un wait per a que acabin els fills */
    wait(&status);   
    wait(&status);    
    
    return 0;    
}