#include <stdio.h>
#include <stdlib.h>

void fillArray(int buf[]) {
  printf("In fillArray: sizeof(buf)=%d\n", (int) sizeof(buf));
}

int main(void) {
  int buf[10];
  printf("In main: sizeof(buf)=%d\n", (int) sizeof(buf));
  fillArray(buf);

  return EXIT_SUCCESS;
}
