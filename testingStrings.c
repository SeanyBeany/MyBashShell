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
    /**
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

        cmd = fgets(buffer, BUFSIZE, stdin);
        //tokenizes the command string
        str = cmd;
        */

    cmd = fgets(buffer, BUFSIZE, stdin);
    char* token;
    char* rest = cmd;
    while ((token = strtok_r(rest, " ", &rest)))
        printf("%s\n", token);
    return (0);

}


