// Some simple macros for error reporting and termination
// FABR 06.2012
//
// For better techniques that use a translation of error codes to
// clear text error messages or even provide a stack for accumulating
// cleanup routines see
//
// Michael Kerrisk: The Linux Programming environment, ISBN 978-1-59327-220-3
// and even more sophisticated
// Marc Rochkind: Advanced UNIX Programming, ISBN 978-0-13-141154-8
//

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT_NO_ERRNO(msg) {\
    fprintf(stderr,"%s\n",msg);\
    exit(EXIT_FAILURE);\
 }

#define ERR_EXIT(msg) {\
    perror(msg);\
    exit(EXIT_FAILURE);\
 }

#define ERR_EXIT_ON_NULL(exp, msg) {\
    if (NULL == (exp)) {\
        perror(msg);\
        exit(EXIT_FAILURE);\
    }\
 }

#define ERR_EXIT_ON_NEG1(exp, msg) {\
    if (-1 == (exp)) {\
        perror(msg);\
        exit(EXIT_FAILURE);\
    }\
 }

#define ERR_EXIT_ON_NOT0(exp, msg) {\
    if (0 != (exp)) {\
        perror(msg);\
        exit(EXIT_FAILURE);\
    }\
 }

#define ERR_EXIT_ON_TRUE(exp, msg) {\
    if ((exp)) {\
        perror(msg);\
        exit(EXIT_FAILURE);\
    }\
 }

#define ERR_EXIT_ON_FALSE(exp, msg) {\
    if (!(exp)) {\
        perror(msg);\
        exit(EXIT_FAILURE);\
    }\
 }
