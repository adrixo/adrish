#include "adrish.h"
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

int adrish(){

  int loopFlag = 1;
  char ** command;

  //Start the main loop
  while(loopFlag){
    //Print the shell info and read a command
    printUserAndDir();

    command = readLineAndSplit();

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

char ** readLineAndSplit(){

  int c, position = 0;
  int bufsize = LINE_BUFFER;

  char * line = calloc(bufsize, sizeof(char));
  if(line == NULL){
    printf("ReadLine: calloc error.");
    exit(EXIT_FAILURE);
  }

  char **args = calloc(bufsize, sizeof(char));
  if(args == NULL){
    printf("ReadLine: calloc error.");
    exit(EXIT_FAILURE);
  }

  /*"scanf" area*/
  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      line[++position] = '\0';
      addToHistory(line);
      break;
    } /*else if ( c == '\r'){ //Unecesary part becouse bash has a buffer
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

  /*Split area*/
  args[0] = strtok(line, " \t\r\n\a");
  position = 0;
  while(args[position]!=NULL){
    position++;
    args[position] = strtok(NULL, " \t\r\n\a");
  }
  args[position]=NULL;

  return args;
}

void addToHistory(char * command){
  char * env_home;
  char * cpy_home;
  FILE * f;
  char hfile[20] = "/.adrish_history";

  env_home = getenv("HOME");
  cpy_home = calloc(sizeof(env_home),sizeof(char));
  if(cpy_home==NULL){
    printf("Error: addToHistory calloc.\n");
    return;
  }

  strcpy(cpy_home,env_home);
  strcat(env_home, hfile);        // char * strcpy (dest + strlen (dest), src);

  f = fopen( env_home , "a");
  if(f == NULL) {
    printf("Error: addToHistory opening file.\n");
    strcpy(env_home,cpy_home);
    return;
  }
  fprintf(f, "%s\n", command);

  strcpy(env_home,cpy_home);
  fclose(f);
}

int execute(char ** command){
  int pid, status;

  if(strcmp(command[0],"exit")==0)
    return 0;

  pid = fork();
  switch(pid){
    case -1:
      printf ("Error: child from fork during execution. \n");
      break;
    case 0:
      if(execvp(command[0], command) == -1){
        printf("Error: execvp problem, command not found?\n");
      }
      exit(EXIT_SUCCESS); //the child should die
      break;
    default:
      waitpid(pid, &status, WUNTRACED);
  }

  return 1;
}
