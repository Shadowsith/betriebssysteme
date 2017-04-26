#include <stdio.h>
#define SIZE 512

int main(void) {
  FILE* fp;
  char buf[SIZE];
  size_t nr;

  fp = fopen("DerWerwolf.txt","r");
  if (fp == NULL) {
    return 1;
  } else {
    nr = fread(buf, sizeof(char), SIZE, fp);       
    if(nr > 0) {
      printf("%d characters read\n", (int) nr);
      buf[nr-1] = '\0';
      printf("%s\n",buf);
    } else {
      printf("Unable to read from file descriptor\n");
    }
    fclose(fp);
    return 0;
  }
}
