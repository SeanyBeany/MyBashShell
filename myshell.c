#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myshell.h"

//Main function
int main(int argc, char *argv[]){ 
    
    int len; //length of entered command
    char buffer[BUFSIZE];	// roomfor 1000 characters
    char* tokens[BUFSIZE]; //holds an array of tokenized strings of the original users input
    char *cmd; //pointer to entered command
    int tokenCounter = 0; //Counter for tokens
    int end = 0; //Used to keep loop going until user inputs exit command
    int numberOfPipes = 0; //Number of pipes in the command, default is 0
    while(!end){      
         
        printDirectory();
        cmd = fgets(buffer, BUFSIZE, stdin); //Get user input;
        while(strcmp(cmd, "\n") == 0){ //If user input is empty(\n), keep getting input
            printDirectory();
            cmd = fgets(buffer, BUFSIZE, stdin);
        }
        
        if(cmd != NULL){ 
            //check for the newline character and overwrite with \0
            len = strlen(buffer);
            if(buffer[len-1] == '\n'){
                buffer[len-1] = '\0';
            }
        }

        tokenizeString(buffer, cmd, tokens);
        tokenCounter = countTokens(tokens); //Count the number of tokens
        numberOfPipes = pipeCounter(tokens); //Count the number of pipes
        //cd command implementation
        if(strcmp(tokens[0], "cd") == 0){
            chdir(tokens[1]);
        }
        
        //exit commands implementation
        else if(strcmp("exit", tokens[0]) == 0){
            end = 1;
        }

        else if(numberOfPipes == 0){
            execute(tokens, tokenCounter);
        }
        else {
            executeWithPipes(tokens, tokenCounter, numberOfPipes);
        }
        
        
        

        
    }
    //End program
    printf("Program has successfully exited\n");
    return 0; 
}

void printDirectory() {
    char cwd[BUFSIZE]; //Contains path to current working directory
    static int percentage = 37; //Ascii value for percentage
    getcwd(cwd, sizeof(cwd)); //Get the current working directory
    printf("%s%c", cwd, percentage); //Print the current working directory
}

char** tokenizeString(char* buf, char* cmd, char** tokens) {
    char* p = cmd; //Pointer to the user input string
    int i; //Index variable for loop
    for(i = 0; i < BUFSIZE; i++){
        tokens[i] = strtok_r(p, " ", &p);
        if(tokens[i] == NULL){
            break;
        }
    }
    return tokens;
}

int countTokens(char** tokens){
    int i; //Index variable for loop
    int end = 0; //Variable to stop the loop
    int tokenCounter = 0; //Counter for tokens
    for(i = 0; end != 1; i++){
        if(tokens[i] == NULL){
            end = 1;
        }
        tokenCounter++;
    }
    return tokenCounter;
}

/**Function that returns the number of pipes ("|") in an array of tokens
*  Input is a pointer to an array of tokens and the buf and cmd parameters are so that it
* can use strcmp without segfaulting since there would be an invalid memory access when using strcmp
*/
int pipeCounter(char** tokens){
    int i; //Index variable for loop
    int end = 0; //Variable to stop the loop
    int pipeCounter = 0; //Counter for tokens
    char* p = tokens[0];
    for(i = 0; end != 1; i++){
        if(tokens[i] == NULL){
            end = 1;
        }
        if(strcmp(p, "|") == 0){
            pipeCounter++;
        }
        p = tokens[i];
    }
    return pipeCounter;
}



//Method that executes a command with no piping required
//Parameter is the array of tokenized strings and the total number of tokenized strings
void execute(char** tokens, int numberOfTokens){
    char* command[COMMAND_LIMIT]; //Array of all the strings to execute a system command with execvp
    int status; // status of parent or child process
    int ret_code; // return code
    int i; // variable used for loop
    pid_t pid; //pid of the current child or parent process

    
    pid = fork(); //create child process
    if(pid == -1){ //fork failed, therefore print error message
        perror("error creating child process");
    }
    // Child process
    if(pid == 0){
        //Puts every string together in an array to execute command
        for(i = 0; i< numberOfTokens; i++){
            command[i] = tokens[i];
        }
        command[numberOfTokens] = NULL;//The array needs to terminate with NULL
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

//Method that executes a command with no piping required
//Parameter is the array of tokenized strings and the total number of tokenized strings
void executeWithPipes(char** tokens, int numberOfTokens, int numberOfPipes){
    int fd[2]; //This int array contains file descriptors
    char* command[COMMAND_LIMIT]; //Array of all the strings to execute a system command with execvp
    int pipe_ret_code; // return code of pipe command
    pid_t child; //pid of the first child or the parent process
    pid_t child2; //pid of the second child process or the parent process
    int status; //status of the process running
    int ret_code; //return code of the process
    int i; //Variable for loop
    int end = 0; //Variable to end loop
    //int tokenIndex = 0; //Used to keep track of index in tokens array
    char* owning[COMMAND_LIMIT]; //Array of owning tokens
    
    
   
    

    pipe_ret_code = pipe(fd); //Beginning piping
    if (pipe_ret_code == 1){ //Error protection against a pipe failure
        printf("error opening the pipe");
    }

    child = fork(); //create the first child process
    if(child == -1){
        perror("error creating the child process");
    }

    if(child == 0) { //First child work
        /**
         * Duplicate the write side of the pipe for STDOUT
         * so that we can connect child 1 and 2 together and
         * since the pipe is duplicated we should close the original pipe
        */
        ret_code = dup2(fd[WRITE], STDOUT); 
        if(ret_code == -1){ //error checking the dup2 method
            perror("error with dup2 command:");
            exit(-1);
        }
        close(fd[WRITE]);
        close(fd[READ]);

        //Puts every string together in an array to execute command
        char* p = tokens[0];
        for(i = 0; strcmp(p, "|") != 0; i++){
            p = tokens[i];
        }
        
        
        
        //owning[0] = "ping";
        //owning[1] = "-c";
        //owning[2] = "5";
        //owning[3] = "google.com";
        tokens[4] = NULL;

        ret_code = execvp(tokens[0], tokens);
        if(ret_code == -1){ //error checking the execvp method
            perror("error with execvp command:");
            exit(-1);
        }
        
        else{
            exit(0);
        }
    }

    child2 = fork(); //create the second child process
    if(child2 == -1){
        perror("error creating the second child process");
    }

    if(child2 == 0) { //Second child work
        /**
         * Same idea with child 2
         * Duplicate the read side of the pipe with STDIN so that we can connect 
         * child 1 and 2 together and this time since we linked the opposite side 
         * of the pipe, the two pipes are properly connected
         * since the pipe is duplicated again we close the original pipe 
         * on the second child processes end
        */
       ret_code = dup2(fd[READ], STDIN);
       if(ret_code == -1){ //error checking the dup2 method
            perror("error with dup2 command:");
            exit(-1);
        }

        close(fd[READ]);
        close(fd[WRITE]);
        owning[0] = "grep";
        owning[1] = "rtt";
        owning[2] = NULL;

        
        ret_code = execvp(owning[0], owning);
        if(ret_code == -1){ //error checking the execvp method
            perror("error with execvp command:");
            exit(-1);
        }
        else{
            exit(0);
        }
    
    }

    else{
        close(fd[READ]); //Close both sides of the pipes for the parent process 
        close(fd[WRITE]);
        wait(&status);
        if(status == -1){
            perror("error with first child process");
        }
        wait(&status);
        if(status == -1){
            perror("error with second child process");
        }
    }
}
