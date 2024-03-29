#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 100

int main(int argc, char *argv[]){
    
    int ret_code;		// return code
    int len;		// length of entered command
    char buffer[BUFSIZE];	// room for 80 chars plus \0
    char *cmd;		// pointer to entered command
    int pid;
    char cwd[BUFSIZE];
    static int percentage = 37; //Ascii value for percentage
    
    getcwd(cwd, sizeof(cwd));
    printf("%s%c", cwd,percentage);
    cmd = fgets(buffer, BUFSIZE, stdin);
    
    while(cmd != NULL){
        // Print a prompt and read a command from standard input
        pid=fork();
        if(pid!=0){
            /* parent process executes here */
            wait(NULL);
        }       
        else{
            if(cmd != NULL){
            // check for the newline character and overwrite with \0
            len = strlen(buffer);
            if(buffer[len-1] == '\n'){
                buffer[len-1] = '\0';
            }
            // execute the command
            ret_code = execlp(cmd, cmd, NULL);
            if(ret_code != 0 && strcmp(cmd, "\n") == 0){
                printf("Error executing %s.\n", cmd);
                exit(0);
                }
            }
        }
        getcwd(cwd, sizeof(cwd));
        printf("%s%c", cwd, percentage);
        cmd = fgets(buffer, BUFSIZE, stdin);

        while(strcmp(cmd, "\n") == 0){
            printf("%s%c", cwd, percentage);
            cmd = fgets(buffer, BUFSIZE, stdin);
        }
    }
    
    printf("\nAll done.\n");
} // main 

