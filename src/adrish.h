
#define LINE_BUFFER 10
/*
 * shell main loop
 */
int adrish();

void printUserAndDir();
char ** readLineAndSplit();
void addToHistory(char * command);

int execute(char ** command);
