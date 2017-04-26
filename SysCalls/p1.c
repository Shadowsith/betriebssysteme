// A simple program that prints the content of a global variable
// onto the console
// This program prints variable g1

#include <stdio.h>
#include <stdlib.h>  // for EXIT_SUCCESS

#include <sys/types.h>
#include <unistd.h>


int g1 = 0x11223344;
int g2 = 0x66778899;

int main(void) {
  printf("PID=%d\taddress of g1=%p\tvalue of g1=0x%x\n",(int) getpid(), &g1,g1);

  // stop until key is pressed
  printf("Bitte Taste druecken.\n");
  getchar();

  return EXIT_SUCCESS;
}

