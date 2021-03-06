//TODO: buffers?
//http://man7.org/linux/man-pages/man3/setbuf.3.html

#include "adrish.h"
#include "commands.c"
#include <unistd.h>
#include <string.h>

#include <errno.h>
//  Each process has his own predefined variable:
//   extern int errno;
//  I'm using perror to notify problems, but i could use char * strerror(int errnum);
//    fprintf(stderr, "%s : error: %s", argv[0], sterror(errno))

#include <sys/types.h>
#include <sys/wait.h>

/*
 * Main shell loop:
 *    1. Print adrish information.
 *    2. Read user input
 *    3. Execute command
 */
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

/*
 * Prints the username and the current directory
 * forming the promp.
 */
void printUserAndDir(){
  char cwd[512];
  char user[32];
  getlogin_r(user, sizeof(user));
  getcwd(cwd, sizeof(cwd));
  printf("%s@%s: ", user, cwd);
}

/*
 * 1. Reads from the user's input
 * 2. and split the whole string into arguments.
 * returning them in an array.
 */
char ** readLineAndSplit(){

  int c, position = 0;
  int bufsize = BUFSIZ;

  char * line = calloc(bufsize, sizeof(char));
  if(line == NULL){
    perror("ReadLine: calloc error.");
    exit(EXIT_FAILURE);
  }

  char **args = calloc(bufsize, sizeof(char));
  if(args == NULL){
    perror("ReadLine: calloc error.");
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

    //If we dont have enough space in the line, realloc it
    if (position >= bufsize) {
      bufsize += BUFSIZ;
      line = realloc(line, bufsize);
      if (line == NULL){
        perror("ReadLine: Buffer error.");
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

/*
 *
 */
void addToHistory(char * command){
  char * env_home;
  char * cpy_home;
  FILE * f;
  char hfile[20] = "/.adrish_history";

  env_home = getenv("HOME");
  cpy_home = calloc(sizeof(env_home),sizeof(char));
  if(cpy_home==NULL){
    perror("Error: addToHistory calloc.\n");
    return;
  }

//we need to make a copy of env_home becouse the pointer
//points to the env. var. and replace it again later.
  strcpy(cpy_home,env_home);
  strcat(env_home, hfile);        // char * strcpy (dest + strlen (dest), src);

  f = fopen( env_home , "a");
  if(f == NULL) {
    perror("Error: addToHistory opening file.\n");
    strcpy(env_home,cpy_home);
    return;
  }
  fprintf(f, "%s\n", command);

  strcpy(env_home,cpy_home);
  fclose(f);
}

/*
 * This function create the subprocess responsible for executing
 * the command, returning 0 if exit is typed.
 * We can't call 'cd' directly and probably we can add other adrish
 * functions in the future, so first it calls other functions existing
 * in the program itself.
 */
int execute(char ** command){

  if(comandExists(command)){
    return 1;
  }

  if(strcmp(command[0],"exit")==0)
  return 0;

  int pid, status;


  pid = fork();
  switch(pid){
    case -1:
      perror ("Error: child from fork during execution. \n");
      break;
    case 0:
      if(execvp(command[0], command) == -1){
        printf("Error: execvp problem, command not found?\n");
      }
      exit(EXIT_SUCCESS); //the child should die
      break;
    default:
      waitpid(pid, &status, WUNTRACED); //waiting for the child death to continue
  }

  return 1;
}
