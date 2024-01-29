#include "stdint.h"
#include "stdio.h"

void main(){
    char buffer[100];
    char buffer2[5];
    uint32_t len;
    putstr("[-] get input string1\n");
    len = getstr(buffer2, 5);
     putstr("\n[-] get input string1\n");
    len = getstr(buffer, 100);
    putstr("[-] your input 1: \n");
    putstr(buffer2);
    putstr("\n[-] your input 2: \n");
    putstr(buffer);
}

