// A simple program that prints the content of a global variable
// onto the console
// This program prints variable g2

#include <stdio.h>
#include <stdlib.h>  // for EXIT_SUCCESS

#include <sys/types.h>
#include <unistd.h>


int g1 = 0x11223344;
int g2 = 0x66778899;

int main(void) {
  printf("PID=%d\taddress of g2=%p\tvalue of g2=0x%x\n",(int) getpid(), &g2,g2);

  // stop until key is pressed
  printf("Bitte Taste druecken.\n");
  getchar();

  return EXIT_SUCCESS;
}

