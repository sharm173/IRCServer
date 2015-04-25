#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UserList.h"

//
// Initialize a linked list
//
void llist_init(LinkedList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
void llist_print(LinkedList * list) {
	
	ListNode * e;

	if (list->head == NULL) {
		printf("EMPTY\n");
		return;
	}

	//printf("{");

	e = list->head;
	while (e != NULL) {
		printf("%s\n", e->user);
		e = e->next;
		if (e!=NULL) {
		//	printf(", ");
		}
	}
//	printf("}\n");
}

void llist_printmsg(LinkedList * list, int last) {

        ListNode * e;

        if (list->head == NULL) {
                printf("EMPTY\n");
                return;
        }
 
       // printf("{");
  	int i = 0;
        e = list->head;
        while (e != NULL) {
	if(i >= last) {
                printf("%s: %s\n", e->user, e->pwd);
		}    
		i++;
	    	e = e->next;
                if (e!=NULL) {
                      //  printf(", ");
                }
        
	}
       // printf("}\n");
}

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list,const char *user,const char *pwd) {
	// Create new node
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->user=strdup(user);
	n->pwd = strdup(pwd);
	
	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}


// Returns true if the value exists in the list.

int llist_exists(LinkedList * list,const char *user) {
ListNode * e;
int flag = 0;          
 e = list->head;
        while (e != NULL) {
                if(strcmp((e->user),user) == 0) {
            flag = 1;
}
                e = e->next;
              }
            
                

	return flag;
}

//checkpwd
int llist_checkpwd(LinkedList * list,const char *user,const char *pwd) {
ListNode * e;
int flag = 0;
 e = list->head;
        while (e != NULL) {
                if(strcmp((e->user),user) == 0 && strcmp((e->pwd),pwd) == 0) {
            flag = 1;
}
                e = e->next;
              }



        return flag;



}





//
// It removes the entry with that value in the list.
//

int llist_remove(LinkedList * list,const char *user,const char *pwd) {
	
if(!llist_checkpwd(list, user, pwd))
	return 0;

ListNode * e;
e = list->head;
if(strcmp((e->user),user) == 0) {
list->head = e-> next;
return 1;
}
while (e -> next != NULL) {
 
    if(strcmp(((e->next)->user), user)) {
       e->next = e->next->next;
       return 1;
}
                e = e->next;
              }


   return 1;


}
//
// It stores in *value the value that correspond to the ith entry.
// It returns 1 if success or 0 if there is no ith entry.
//
/*
int llist_get_ith(LinkedList * list, int ith, int * value) {
ListNode * e;
e = list -> head;
int i;
for(i=0; i < ith; i++) {
if(e -> next != NULL) {
e = e-> next;
}
else {
return 0;
}

}
*value = e->value;
	return 1;
}



//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_remove_ith(LinkedList * list, int ith) {
	ListNode * e;
e = list -> head;
int i;
for(i=0; i < ith-1; i++) {
if(e -> next != NULL) {
e = e-> next;
} 
else {
return 0;  
}  
}

if(e->next == NULL) {
return 0;
}

e->next = e->next->next;
return 1;

}


*/
//
// It returns the number of elements in the list.

int llist_number_elements(LinkedList * list) {
ListNode * e;
e = list -> head;
int i=0;
while((e != NULL)) {
i++;
if((e -> next)== NULL) {
break;
}
else {
e = e->next;
}
}
	return i;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// value1\n
// value2\n
// ...
//

int llist_save(LinkedList * list, char * file_name) {
FILE * fd = fopen(file_name, "a");

        if (fd == NULL) {

//          printf("Could not open file %s\n", file_name);

          return 0;

        }
ListNode * e;
e = list->head;
while(e != NULL) { 
fprintf(fd,"%s %s\n",e->user, e->pwd);
e = e -> next;
}

fclose(fd);

return 0;
}

//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
int llist_read(LinkedList * list, char * file_name) {
//printf("First Line");
int c;
ListNode * e;
FILE * fd = fopen(file_name,"r");
list->head = NULL;
 if(fd == NULL) {
//printf("Err: Could not open file");
return 0;
}
 int i = 0;
char *user = (char*) malloc(sizeof(char)*30);
char *pwd = (char*) malloc(sizeof(char)*30);
//printf("Err: Could open file");

fscanf (fd, "%s", user);
fscanf (fd, "%s", pwd);
//printf("info:%s, %s ", user, pwd);
    
  while (!feof (fd))
    {  
     llist_add(list, user, pwd);
        fscanf (fd, "%s", user);     
fscanf (fd, "%s",  pwd);      
    }

        fclose(fd);


	return 1;

}


//
// It sorts the list. The parameter ascending determines if the
// order si ascending (1) or descending(0).
//
void llist_sort(LinkedList * list) {
ListNode * e;
e = list->head;
int flag=0;

while(1)       {
flag = 0;
e = list->head;
while(e->next != NULL) {
if(strcmp(e->user, e->next->user) > 0) {
char * temp = (char*)malloc((strlen(e->user)+1)*sizeof(char));
strcpy(temp,e->user);
//e->user = strdup(e->next->user);

strcpy(e->user,e->next->user);
//e->next->user = strdup(temp);

strcpy(e->next->user,e->user);

free(temp);
flag = 1;
}
e = e->next;
}
if(!flag) {
return;
}
}


/*
if(ascending == 0) {
    while(1)       {
flag = 0;
e = list->head;
while(e->next != NULL) {
if(e->value < e->next->value) {
int temp = e->value;
e->value = e->next->value;
e->next->value = temp;
flag = 1;
}
e = e->next;
}
if(!flag) {
return;
}  
} 

}

*/

}

//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
/*
int llist_remove_first(LinkedList * list, int * value) {
 ListNode * e;
e = list->head;
if(e == NULL) {
return 0;
}
else {
*value = e-> value;
list->head = e->next;
e = NULL;
free(e);
	return 1;

}
}
*/
//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list) {
ListNode * e;
e = list->head;
if(e == NULL) {
return 0;
}
else {
while(e->next->next != NULL) {
e = e->next;
}  
//*value = e->next->value;
e->next = NULL;
free(e->next);
return 1;
}
}
//
// Insert a value at the beginning of the list.
// There is no check if the value exists. The entry is added
// at the beginning of the list.
/*
void llist_insert_first(LinkedList * list, int value) {

//llist_add(list, value);

}
*/
//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
/*
void llist_insert_last(LinkedList * list, int value) {
ListNode * n = (ListNode *) malloc(sizeof(ListNode));
        n->value = value;
ListNode * e;
e = list->head;
if (e == NULL) {
llist_add(list, value);
}
else {
while(e->next != NULL) {
e = e->next;
}
e->next = n;
n->next = NULL;
}
}
*/
//
// Clear all elements in the list and free the nodes
/*
void llist_clear(LinkedList *list)
{
while(1) {
 ListNode * e; 
e = list->head;
if(e == NULL) {
break;
}
else {
list->head = e->next;
e = NULL;
free(e);                

}
}
}
*/
