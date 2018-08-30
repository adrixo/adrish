#include <errno.h>


if( some_sistem_call(param1,param2) < 0 ){
  fprintf(stderr, "%s: %s (%s %d): some_sistem_call(%d, %d) failed: %s\n",
    argv[0], __func__, __FILE__, __LINE__,
    param1, param2, strerror(errno));
    return 1;
}
