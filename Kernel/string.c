// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "_string.h"
#include <stdint.h>

void _str_cpy(char * dest, char * src, int n){
   while ( *src != '\0' && *src != ' ' && n > 0 ){
        *dest++ = *src++;
        n--;
    }
    *dest = '\0';
}

int _str_cmp(char *str1, char *str2){
	while( ( *str1 != '\0' && *str2 != '\0' ) && *str1 == *str2 )
    {
        str1++;
        str2++;
    }

    if(*str1 == *str2)
    {
        return 0; // strings are identical
    }

    else
    {
        return *str1 - *str2;
    }
}

uint64_t _str_len(const char * str){
	uint64_t ans = 0;
    while(str[ans] != '\0'){
        ans++;
        //str++;
    }
	return ans;
}
