#include <stdio.h>

/*
gcc -D_FORTIFY_SOURCE=2 -O2
./a.out
%n
*** %n in writable segment detected ***
*/
int main(){
  char buf[256];
  fgets(buf, sizeof(buf), stdin);
  printf(buf);
}