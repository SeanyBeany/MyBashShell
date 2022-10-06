#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 100
#define EXIT_SUCCESS 0

int main(int argc, char *argv[]){
    
    //int ret_code;		// return code
    int len;		// length of entered command
    char buffer[BUFSIZE];	// room for 80 chars plus \0
    char *cmd;		// pointer to entered command
    //int pid;
    char cwd[BUFSIZE];
    static int percentage = 37; //Ascii value for percentage
    
    
    
    while(1){       

        getcwd(cwd, sizeof(cwd));
        printf("%s%c", cwd, percentage);
        cmd = fgets(buffer, BUFSIZE, stdin);

        while(strcmp(cmd, "\n") == 0){
            getcwd(cwd, sizeof(cwd));
            printf("%s%c", cwd,percentage);
            cmd = fgets(buffer, BUFSIZE, stdin);
        }
        
        if(cmd != NULL){
            // check for the newline character and overwrite with \0
            len = strlen(buffer);
            if(buffer[len-1] == '\n'){
                buffer[len-1] = '\0';
            }
        }

        
        
        
        
        //tokenizes the command string
        char *p = cmd;
        char* tokens[BUFSIZE];

        for(int i = 0; i < BUFSIZE; i++){
            tokens[i] = strtok_r(p, " ", &p);
            if(tokens[i] == NULL){
                break;
            }
        }
        
        //cd command implementation
        if(strcmp(tokens[0], "cd") == 0){
            chdir(tokens[1]);
        }
        
        //exit commands implementation
        if(strcmp("exit", tokens[0]) == 0){
            printf("Program has successfully exited\n");
            return EXIT_SUCCESS;
        }

        
        
    }

    
    /**
    int what = strcmp(tokens[0], "ls");
    printf("%d",what);
    ret_code = execlp(tokens[0], tokens[0], NULL);
    */
    return EXIT_SUCCESS;

}

