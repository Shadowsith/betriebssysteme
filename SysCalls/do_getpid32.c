// 01.04.12: FABR
//
// ATTENTION: do not run this code on a 64bit linux
//
// Notes:
// 1) Syscall numbers are different for 32bit and 64bit systems
//
// getpid:
// 32bit: 20 or 0x14
// 64bit: 39 or 0x27
//
// 2) Assembler commands for doing a syscall are different

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int pid;

int main() {
  // Do it yourself: directly via asm code
  __asm__(
      "movl $20, %eax    \n"
      "int $0x80  \n"
      "movl %eax, pid    \n"
      );
  printf("Via assembler code: pid is %d\n", pid);

  // Use the normale way via syscall interface
  printf("Via syscall: pid is %d\n", (int) getpid());

  return 0;

}

