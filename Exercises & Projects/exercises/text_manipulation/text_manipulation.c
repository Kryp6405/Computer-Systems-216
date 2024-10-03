#include "text_manipulation.h"
#include <stdio.h>
#include <string.h>

int right_align(const char * src, char *res, int len){
    if((src != NULL && src[0] != '\0') && res != NULL){
        int temp_len = len, i = 0, lead = 0, trail = strlen(src)-1, src_len = 0, start, end;
        /*Find loc of last character in end of src*/
        while(src[trail] == ' '){
            trail--;
        }
        /*Add all len # of spaces to res*/
        while(temp_len > 1){
            res[i++] = ' ';
            temp_len--;
        }

        while (src[lead] != '\0') {
            /*Loop throguh src till word starts (i.e. skip any leading spaces)*/
            if (src[lead] == ' ') {
                lead++;
                continue;
            } else {
                /*Copy characters to end of res (starting from trailing character in src) */
                while (trail >= lead) {
                    if(len-src_len-2 < 0){
                        int i;
                        for(i = len-src_len-1; i < len-2; i++){
                            res[i] = '\0';
                        }
                        return FAILURE;
                    }
                    res[len-src_len-2] = src[trail];
                    src_len++;
                    trail--;
                }

                break;
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}


int compact(char * str, int * out){
    if(str != NULL && str[0] != '\0'){
        int i = 0, j = 0;
            /*Loop throguh src till word(s) start (i.e. skip any spaces)*/
        while(str[j] != '\0'){  
            if(str[j] == ' '){
                j++;
                continue;
            }
            else{
                /*Update the beginning of the string ONLY with characters 
                (i.e. all words get compiled to beginning of str)*/
                str[i++] = str[j++];

                /*If the next character is a space or null byte, it means we are at end of the word*/
                if(out !=  NULL && (str[j] == ' ' || str[j] == '\0'))
                    (*out)++;
            }
        }
        str[i] = '\0';
        return SUCCESS;
    }
    return FAILURE;
}
