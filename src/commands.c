/*
 * This file only contains adrish's native functions
 */

#include "commands.h"
#include <unistd.h>
#include <string.h>

/*
 *  Checks if the command exists in adrish and, in that case, executes it
 */
int comandExists(char ** command){
  if(strcmp(command[0], "cd")==0){
    cd(command[1]);
    return 1;
  }
    return 0;
}

int cd(char * path){
  int ret;

  if(path == NULL){
    printf("Error: No path.\n");
    return 1;
  } else {
    ret = chdir(path);
    if(ret == -1)
      printf("Error: maybe path doesn't exists.\n");

    return 1;
  }
}
