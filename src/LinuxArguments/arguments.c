#include <stdio.h>
#include <unistd.h>

void main(int argc, char *argv[]){

  /*
   * getopt() getopt_long() / unistd.h
   * int getopt(int argc, char *const argv[], const char *optstring);
   *   If you spect an argument wiht a char, you need ':'
   * getopt define various variables:
   *  char *optarg  | int optind  | int opterr  |int optopt
   */
   int optionCharacter;
   char *b_opt_arg;

   /* Start optstring with : is interesting*/
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
}
