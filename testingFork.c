#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 100
#define EXIT_SUCCESS 0
#define COMMAND_LIMIT 20

int main(int argc, char *argv[]){
    char buffer[BUFSIZE];

    pid_t pid;
    int status;
    char *name[3];
    while(1){
        pid = fork(); 
        if(pid == -1){
            perror("fork");
            return 2;
        }

        if(pid == 0){
            name[0] = "echo";
            name[1] = "word";
            name[2] = NULL;
            execvp("echo", name);
            printf("word");
            fgets(buffer, BUFSIZE, stdin);
            exit(0);
        }
        else{
            wait(&status);
            printf("Status : %d\n", WEXITSTATUS(status));
        }
        printf("owning");
        return(0);
    }
}
    
