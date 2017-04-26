#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 512

int main(void) {
  char buf[SIZE];
  int fd, nr;

  fd = open("DerWerwolf.txt", O_RDONLY);
  if ( fd < 0 ) {
    return 1;
  } else {
    nr = read(fd, buf, SIZE);
    if(nr > 0) {
      // doing formatted output at low level would be cumbersome 
      printf("%d characters read\n", (int) nr);

      // However, just writing an already prepared buffer is easy
      write(1,buf,nr); // ignore result of write

      buf[0] = '\n';
      write(1,buf,1); // ignore result of write
    } else {
      printf("Unable to read from file descriptor\n");
    }

    close(fd);
    return 0;
  }
}
