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

#ifndef _NGETS_H
#define _NGETS_H

extern bool ngets(char *buffer, int bufsize);

#endif  // #define _NGETS_H
