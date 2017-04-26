// A function for reading entire lines from stdin
// It is a save variant of gets()
//
// 15.04.2012: FABR

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Read a string from stdin into buffer
// Parameters:
// char *buffer: pointer to an already allocated buffer
// int bufsize: size of the buffer
//
// Return ivalue:
// true on success, meaning that something could be read from stdin
// false on error, meaning that fgets returned NULL
//
// The caller is free to inspect the errno variable
// We do not alter the value that resulted from the call to fgets
//

bool ngets(char *buffer, int bufsize) {
  char *res = fgets(buffer,bufsize,stdin);
  if (res != NULL) {
    // fgets() guarantees at least one \0 in the buffer
    int len = strlen(buffer);
    if (len > 0) {
      if (buffer[len -1] != '\n') {
        // Attempt for buffer overrun; discard rest of input buffer
        while (fgetc(stdin) != '\n');
      }
      // In all cases delete character prior to \0
      // This is usually a \n if we got no buffer overrun
      buffer[len-1] = '\0';
    }
  }
  return (res != NULL);
}
