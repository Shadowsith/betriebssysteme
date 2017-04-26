// 01.04.12: FABR
//
// ATTENTION: do not run this code on a 32bit linux
//
// Notes:
// 1) Syscall numbers are different for 32bit and 64bit systems
//
// getpid:
// 32bit: 20  or 0x14
// 64bit: 39  or 0x27
//
// 2) Assembler commands for doing a syscall are different
//
// However, even on a 64bit machine we may use register names
// of smaller architectures.
// E.g. syscall number are stil loaded via
//      "mov $39, %eax    \n"
// and not via
//      "movq $39, %rax    \n"
//
// However, using the second form is also possible

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int pid;

int main() {
  // Do it yourself: directly via asm code
  __asm__(
      "mov $39, %eax    \n"
      "syscall  \n"
      "mov %eax, pid    \n"
      );
  printf("Via assembler code: pid is %d\n", pid);

  // Use the normale way via syscall interface
  printf("Via syscall: pid is %d\n", (int) getpid());

  return 0;

}
