#include <stdlib.h>
#include <stdio.h>

void main(int argc, char *argv[]){

  //configuration
  int optionCharacter;
  char *b_opt_arg;

  while((optionCharacter = getopt(argc, argv, "ab::")) != -1){
   switch (optionCharacter) {
    case 'a':
      break;
    case 'b':
      b_opt_arg = optarg;
      break;
    case ':': /*missing option argument*/
      fprintf(stderr, "%s: option '-%c' is invalid: ignored\n", argv[0], optopt);
      break;
    case '?':
    default:
      fprintf(stderr, "%s: option '-%c' is invalid: ignored\n", argv[0], optopt);
      break;
   }
  }

  //Main loop
  if(adrish())
    printf("Exited");
}
