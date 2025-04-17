#include <stdio.h>
#include <stdlib.h>

// TODO: structs, global variables, etc.
typedef struct Node {
	int value;
	struct Node *next;
} Node;

Node *head=NULL ;

static int insertElement(int value) {
	Node *newNode = malloc(sizeof(Node));
	if (newNode == NULL) {
		return -1;
	}
	newNode->value = value;
	newNode->next = NULL;

	if (head == NULL) {
		head = newNode;
		return value;
	}

	Node *current = head;
	while (current->next != NULL) {
		if (current->value == value) {
			free(newNode); // sauberer Umgang mit bereits allokiertem Speicher
			return -1;
		}
		current = current->next;
	}

	if (current->value == value) {
		free(newNode);
		return -1;
	}

	current->next = newNode;
	return value;
}


static int removeElement(void) {
	if(head==NULL) return -1; // Kein Element
	
	int result = head->value;
	
	if(head->next==NULL){
		free(head);
		head=NULL;
	}else{
		Node *newHead= head->next;
		free(head);
		head=newHead;

	}
	return result;
	
}

int main (int argc, char* argv[]) {
	printf("insert 47: %d\n", insertElement(47));
	printf("insert 11: %d\n", insertElement(11));
	printf("insert 23: %d\n", insertElement(23));
	printf("insert 11: %d\n", insertElement(11));

	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());
	
	// TODO: add more tests
	printf("insert 0: %d\n", insertElement(0));
	printf("remove: %d\n", removeElement());
	printf("insert 0: %d\n", insertElement(0));
	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());

	exit(EXIT_SUCCESS);
}
