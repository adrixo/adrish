#include "adrish.h"

int adrish(){

  int loopFlag = 0;
  char * command;

  //Start the main loop
  while(loopFlag){
    //Print the shell info and read a command
    printf("user@host: ");
    command = readLine();

    //Interpretation and execution
    loopFlag = execute(command);

  }

  free(command);
  return 0;
}

char * readLine(){

  int bufsize = LINE_BUFFER;
  char * line = malloc(sizeof(char) * bufsize);
  if(line == NULL){
    printf("ReadLine: Malloc error.");
    exit(EXIT_FAILURE);
  }
  int position = 0;

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      line[++position] = '\0';
      return line;
    } else if ( c == '\r'){
        printf("%c[2K", 27);
        position--;
        for(int i = 0; i<position; i++){
          printf("%c", line[i]);
        }
      } else {
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

int execute(char * command){
  return 0;
  return 1;
}
