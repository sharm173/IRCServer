
// Data structs for a list of ints

struct ListNode {
	char * user;
	char *pwd;
	struct ListNode * next;
};

typedef struct ListNode ListNode;

struct LinkedList {
	ListNode * head;
};

typedef struct LinkedList LinkedList;

void llist_init(LinkedList * list);
void llist_print(LinkedList * list, int fd);
void llist_printmsg(LinkedList * list, int last, int fd);
void llist_add(LinkedList * list, const char *user,const char *pwd);
int llist_exists(LinkedList * list, const char *user);
int llist_checkpwd(LinkedList * list, const char *user, const char *pwd);
int llist_remove(LinkedList * list, const char *user, const char *pwd);
//int llist_get_ith(LinkedList * list, int ith, const char *user,const char *pwd);
//int llist_remove_ith(LinkedList * list,int ith);
int llist_number_elements(LinkedList * list);
int llist_save(LinkedList * list, char * file_name);
int llist_read(LinkedList * list, char * file_name);
void llist_sort(LinkedList * list);
//void llist_clear(LinkedList *list);

//int llist_remove_first(LinkedList * list);
int llist_remove_last(LinkedList * list);
//void llist_insert_first(LinkedList * list, char *user, char*pwd);
void llist_insert_last(LinkedList * list, const char *user, const char *pwd);
