#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>

#define MAX_INT 18

void _str_cpy(char * dest, char * src, int n);

uint64_t _str_len(const char  *str);

int _str_cmp(char *str1, char *str2);

uint64_t itoa(uint64_t number, char* s);

int _str_toi(char * buffer, int * i);

int hex_str_to_int(char* s, uint8_t **result);

uint64_t atoi(char S[]);

#endif