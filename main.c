/*
 *
 * Author: Shouheng Wu
 * Date: Sep 26, 2014
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "list.h"

int valid_and_trim(char* s){
/*this function examines the token passed to it. It the current token starts is a non-integer that doesn't contain '#', the function returns -3, causing the calling function to ignore the current token and examines the next. If the current token starts with a '#', the function returns -2, meaning that the calling function should ignore the rest of the current line. If the current token is an integer followed immediately by a '#', the function truncates everything after (and including) the #, and then returns -1, meaning that the calling function should take the integer and then ignore the rest of the line. If the current token is a "pure" integer, returns 0.*/

    if(s[0] == '#'){
	return -2;
    }//end if

    if(s[0] != '-' && !isdigit(s[0])){
       return -3;
    }//end if

    int i = 1;
    while(s[i] != '\0'){
       if(s[i] == '#'){
            s[i] == '\0';
            if(i == 1){// in case that the string is "-#"
		return -2;
            }//end if
            else{
            	return -1;
            }//end else
       }//end if
       else if(!isdigit(s[i])){
            return -3;
       }//end if
       i++;
    }//end while

    return 0;

}//end valid_and_trim

double extract_time(const struct timeval *time){

     double time_elapsed = (double) (time->tv_sec);

     time_elapsed += ((double)(time->tv_usec)) / 1000000;
     return time_elapsed;

}//end extract_time

void process_data(FILE *input_file) {
    // !! your code should start here.  the input_file parameter
    // is an already-open file.  you can read data from it using
    // the fgets() C library function.  close it with the fclose()
    // built-in function

    //get the size of the file
    fseek(input_file, 0, SEEK_END);
    long int size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    char *s = malloc((size + 1) * sizeof(char));
    s[size + 1] = '\0'; 

    struct node *head_node_ptr = NULL; //pointer to the first item of the linked list
    struct node **head = &head_node_ptr; //pointer to the pointer to the first item of linked list

	while(NULL != fgets(s, size + 1, input_file)){
		char **line = tokenify(s);
                char **line_start = line;//store the original value of line in order to free it later
		if(NULL == line){//if the line is empty
			continue;
		}//end if
		while(NULL != *line){
			int verification = valid_and_trim(*line);
			if(0 == verification){
				int candidate = atoi(*line);
				list_smart_append(candidate, head);
			}else if(-1 == verification){
				int candidate = atoi(*line);
				list_smart_append(candidate, head);
				break;
			}else if(-2 == verification){
				break;
			}//end else-if

			line++;//this does NOT mean go to the next line; it means go to the next token in the current line

		}//end inner while

		free(line_start);

	}//end outer while 


    free(s);
    list_print_contents(*head);
    list_clear(*head);
    
    //print out program usage
    struct rusage *myusage = malloc(sizeof(struct rusage));
    if(-1 == getrusage(RUSAGE_SELF, myusage)){
        printf("Failed to get program usage information.\n");
	return;
    }//end if
    
    double user_time = extract_time(&(myusage->ru_utime));
    double sys_time = extract_time(&(myusage->ru_stime));
    
    printf("User time: %f\nSystem time: %f\n", user_time, sys_time);

}//end process_data

void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST
int main(int argc, char **argv) {
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);
    return 0;
}
#endif
