#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 100
#define EXIT_SUCCESS 0
#define COMMAND_LIMIT 20

int main(int argc, char *argv[]){

    char *name[4];

    name[0] = "sh";
    name[1] = "-c";
    name[2] = "cd /home/";
    name[3] = NULL;
    execvp("/bin/sh", name);
    printf("word");
}
    
