#ifndef STRING_H_
#define STRING_H_

#include <stdint.h>
void _str_cpy(char * dest, char * src, int n);

int _str_cmp(char *str1, char *str2);

uint64_t _str_len(const char * str);
#endif