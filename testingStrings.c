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
        int ret_code;		// return code
        int len;		// length of entered command
        char buffer[BUFSIZE];	// room for 80 chars plus \0
        char *cmd;		// pointer to entered command
        int pid;
        char cwd[BUFSIZE];
        static int percentage = 37; //Ascii value for percentage
        char* token;
        char* str;

        if(cmd != NULL){
            // check for the newline character and overwrite with \0
            len = strlen(buffer);
            if(buffer[len-1] == '\n'){
                buffer[len-1] = '\0';
            }
        }
        getcwd(cwd, sizeof(cwd));
        printf("%s%c", cwd, percentage);

        
        //tokenizes the command string
        char *p = cmd;
        char* tokens[BUFSIZE];

        for(int i = 0; i < BUFSIZE; i++){
            tokens[i] = strtok_r(p, " ", &p);
            if(tokens[i] == NULL){
                break;
            }
        }
        for(int i = 0; tokens[i] != NULL; i++){
            printf("%s", tokens[i]);
        }
        cmd = fgets(buffer, BUFSIZE, stdin);
    }
    /**
    cmd = fgets(buffer, BUFSIZE, stdin);
    char *p = cmd;
    char *tokens[BUFSIZE];

    for(int i = 0; i < BUFSIZE; i++){
        tokens[i] = strtok_r(p, " ", &p);
        if(tokens[i] == NULL){
            break;
        }
    }
    */
    /**
    int what = strcmp(tokens[0], "ls");
    printf("%d",what);
    ret_code = execlp(tokens[0], tokens[0], NULL);
    */
    return 0;
    
}

