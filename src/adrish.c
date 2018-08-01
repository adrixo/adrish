#include "adrish.h"
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

int adrish(){

  int loopFlag = 1;
  char * command;

  //Start the main loop
  while(loopFlag){
    //Print the shell info and read a command
    printUserAndDir();

    command = readLine();
    addToHistory(command);

    //exec script
    //Interpretation and execution
    loopFlag = execute(command);
    free(command);
  }

  return 1;
}

void printUserAndDir(){
  char cwd[512];
  char user[32];
  getlogin_r(user, sizeof(user));
  getcwd(cwd, sizeof(cwd));
  printf("%s@%s: ", user, cwd);
}

char * readLine(){

  int bufsize = LINE_BUFFER;
  char * line = calloc(bufsize, sizeof(char));
  if(line == NULL){
    printf("ReadLine: Malloc error.");
    exit(EXIT_FAILURE);
  }

  int c, position = 0;

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      line[++position] = '\0';
      return line;
    } /*else if ( c == '\r'){ //Unecesary part in case of non workig del
        printf("%c[2K", 27);  //clear a line
        printUserAndDir();
        position--;
        for(int i = 0; i<position; i++){
          printf("%c", line[i]);
        }
      }*/ else {
        line[position++] = c;
      }

    if (position >= bufsize) {
      bufsize += LINE_BUFFER;
      line = realloc(line, bufsize);
      if (line == NULL){
        printf("ReadLine: Buffer error.");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void addToHistory(char * command){
  //get current home route getenv;
  //create or apend to file
  //close file;
  ;
}


int execute(char * command){
  int pid, status;
  printf("%s. \n", command);
  if(strcmp(command,"exit")==0)
    return 0;

  pid = fork();
  switch(pid){
    case -1:
      printf ("Error: fork in execute. \n");
      break;
    case 0:
      printf("Executing %s\n", command);
      exit(EXIT_FAILURE);
      break;
    default:
      waitpid(pid, &status, 0);
  }
  return 1;
}
