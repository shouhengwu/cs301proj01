#include <stdio.h>
#include <stdlib.h>
#include "list.h"


//CHANGE: change all **head to *head;

void list_clear(struct node *list) {
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp);
    }//end while
}//end list_clear

void list_print_contents(const struct node *list) {

    printf("*** %s ***\n", "List Contents Begin");

    while (list != NULL) {
        printf("%d\n", list->value);
        list = list->next;
    }//end while

    printf("*** %s ***\n", "List Contents End");

}//end list_print_contents



void list_smart_append(int candidate, struct node **head) {
	struct node *appended_node = malloc(sizeof(struct node));
	appended_node->value = candidate;

	
	if(*head == NULL){//case 1: if the linked list is empty, construct head
		*head = appended_node;
		(*head)->next = NULL;
	}//end if
	else if(candidate < (*head)->value){//case 2: if the new node is to become the new head
		struct node *tmp = *head;
		*head = appended_node;
		appended_node->next = tmp;
	}//end if
	else{//case 3: if the new node has to be inserted into the middle of the list or appended to the end of the list
		//KEEP WRITING! It should be (candidate > curr-> value && candidate < curr->next->value)
		struct node *curr = *head;
		while(curr != NULL){
			if(curr->next == NULL){//if we are at the end of list
				curr->next = appended_node;
				appended_node->next = NULL;
				return;
			}//end if

			if(candidate >= curr->value && candidate < curr->next->value){
				struct node* tmp = curr->next;
				curr->next = appended_node;
				appended_node->next = tmp;
				return;
			}//end if

			curr = curr->next;
		}//end while
	
	}//end else

}//end list_smart_append

int get_num_tokens(const char *s){

	char *copy = strdup(s);
	
	//get the number of tokens in the string
	char *token = strtok(copy, " \t\n");

	if(NULL == token){
		free(copy);
		return 0; //the string doesn't contain any tokens
	}//end if
	
	int num_tokens = 1;

	while(true){
		token = strtok(NULL, " \t\n");		
		if(NULL == token){
			break;
		}//end if
		num_tokens++;
	}//end while 

	free(copy);
	return num_tokens;

}//end num_tokens

char** tokenify(const char *s) {


	int num_tokens = get_num_tokens(s);
	if(num_tokens == 0){
		return NULL;
	}//end if

	char *copy = strdup(s);
	
	//make the array
	char **array_head = malloc((num_tokens + 1) * sizeof(char *));

	int i = 0;
	char *token = strtok(copy, " \t\n");
	while(token != NULL){
		*(array_head + i) = strdup(token);
		token = strtok(NULL, " \t\n");
		i++;
	}//end while

	*(array_head + i) = NULL;
	
	free(copy);

	return array_head;
}//end tokenify


