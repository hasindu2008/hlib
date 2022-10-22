/* @file bed.c
**
** read a bed file

MIT License

Copyright (c) 2018 Hasindu Gamaarachchi (hasindu@unsw.edu.au)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "bed.h"

char **read_bed_regions(char *bedfile, int64_t *count){

    FILE *bedfp = fopen(bedfile,"r");
    F_CHK(bedfp,bedfile);

    char* buffer = (char*)malloc(sizeof(char) * (100)); //READ+newline+nullcharacter
    MALLOC_CHK(buffer);

    int64_t reg_capcacity = 1024;
    int64_t reg_i = 0;
    char **reg_list = (char **)malloc(reg_capcacity * sizeof(char *));
    MALLOC_CHK(reg_list);


    size_t bufferSize = 100;
    ssize_t readlinebytes = 0;
    int64_t line_no = 0;


    while ((readlinebytes = getline(&buffer, &bufferSize, bedfp)) != -1) {

        char *ref = (char *)malloc(sizeof(char)*readlinebytes);
        MALLOC_CHK(ref);
        int64_t beg=-1;
        int64_t end=-1;

        //TODO can optimised though strtok etc later
        int ret=sscanf(buffer,"%s\t%ld\t%ld",ref,&beg, &end);
        if(ret!=3 || end<beg){
            ERROR("Malformed bed entry at line %ld",line_no);
            exit(EXIT_FAILURE);
        }

        if(reg_i>=reg_capcacity){
            if(reg_capcacity>1000000){
                WARNING("The region bed file has over %ld regions. To reduce memory usage, you may consider merging bed regions.",reg_i);
            }
            reg_capcacity=reg_capcacity*2;
            reg_list = (char **)realloc((void *)reg_list,reg_capcacity * sizeof(char *));
            MALLOC_CHK(reg_list);

        }

        reg_list[reg_i] = (char *)malloc(sizeof(char)*readlinebytes);
        sprintf(reg_list[reg_i],"%s:%ld-%ld",ref, beg, end);
        reg_i++;


        free(ref);



        line_no++;
    }

    fclose(bedfp);
    free(buffer);
    *count = reg_i;

    if(reg_i == 0){
        ERROR("%s","No regions found in the bed file. Empty Bed file?");
        exit(EXIT_FAILURE);
    }

    return reg_list;
}
