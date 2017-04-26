//  DNS-Abfrage machen
//
// 04.04.12/fabr:

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
    struct hostent *hp;
    char* myname;
    myname = argv[0];
    if(argc != 2) {
        fprintf(stderr,
                "Usage: %s remote_host\n", myname);
        exit(1);
    }

    hp = gethostbyname(argv[1]);
    if(hp != NULL) {
        printf("Official name of %s is %s:\n",argv[1],hp->h_name);
        printf("IP addresses of %s are:\n",argv[1]);
        int i = 0;
        while (hp->h_addr_list[i] != NULL) {
            // Extract ip address given in network order (4 bytes)
            // Set a pointer qp to the first byte
            // Note: instead of %u and mask 0xff you may also use %hhu
            // and *qp, *(qp+1), ...
            char *qp = hp->h_addr_list[i];
            printf("\t%u.%u.%u.%u\n",
                    (*qp)&0xff,
                    (*(qp+1)&0xff),
                    (*(qp+2)&0xff),
                    (*(qp+3)&0xff) );
            i++;
        }
    } else {
        fprintf(stderr,"Unable to obtain IP addresses of host %s\n", argv[1]);
    }

    exit(0);
}
