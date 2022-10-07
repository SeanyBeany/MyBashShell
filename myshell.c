#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1000
#define EXIT_SUCCESS 0
#define COMMAND_LIMIT 100

int main(int argc, char *argv[]){
    
    int ret_code;		// return code
    int len;		// length of entered command
    char buffer[BUFSIZE];	// room for 80 chars plus \0
    char *cmd;		// pointer to entered command
    pid_t pid;  //pid of the current child or parent process
    char* command[COMMAND_LIMIT]; //Array of all the strings to execute a system command with execvp
    int status; // status of parent or child process
    
    
    
    while(1){      
         
        printDirectory();
        cmd = fgets(buffer, BUFSIZE, stdin);
        while(strcmp(cmd, "\n") == 0){
            printDirectory();
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
        int tokenCounter = 0; //Counter for tokens
        for(int i = 0; i < BUFSIZE; i++){
            tokens[i] = strtok_r(p, " ", &p);
            if(tokens[i] == NULL){
                break;
            }
            tokenCounter++;
        }

        //cd command implementation
        if(strcmp(tokens[0], "cd") == 0){
            chdir(tokens[1]);
        }
        
        //exit commands implementation
        else if(strcmp("exit", tokens[0]) == 0){
            printf("Program has successfully exited\n");
            return EXIT_SUCCESS;
        }
    
        else{
            pid = fork(); //create child process
            if(pid == -1){ //fork failed, therefore print error message
                perror("error creating child process");
                return 2;
            }
            // Child process
            if(pid == 0){
                //Puts every string together in an array to execute command
                for(int i =0; i< tokenCounter; i++){
                command[i] = tokens[i];
                }
                command[tokenCounter] = NULL;//The array needs to terminate with NULL
                ret_code = execvp(tokens[0], command); // execute command
                if(ret_code == -1){ //execute failed, so print error message
                    perror("error executing command");
                    exit(1);
                }
                else{ //successful execution: return 0
                    exit(0);
                }
            }
            // Parent process
            else{  
                wait(&status); //Parent is waiting on child process
            }
        }

        
    }
    //Incase of error which causes loop break, print error message
    printf("Error occured, ending program");
    return -1; 
}

void printDirectory() {
    char cwd[BUFSIZE]; //Contains path to current working directory
    static int percentage = 37; //Ascii value for percentage
    getcwd(cwd, sizeof(cwd));
    printf("%s%c", cwd, percentage);   
}