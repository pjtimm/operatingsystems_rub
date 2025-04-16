#include <stdio.h>
#include <stdlib.h>

// TODO: structs, global variables, etc.
typedef struct ListNode
{
	int val;
	struct ListNode *next; //einfache Verkettung
}node_t;

node_t *head = NULL;

static int insertElement(int value) {
	node_t *new_node = malloc(sizeof(node_t));
	if (new_node == NULL) {
        return -1; // malloc failed
    }

	//Werte zuweisen
	new_node -> val = value;
	new_node -> next = NULL;


	if (head == NULL) //leere Liste
	{
		head = new_node;
		return value;
	}
	else
	{
		node_t *current = head; 
		
		//Bis zum Ende der Liste laufen
		while (current -> next != NULL)
		{
			if (current -> val == value) //Wert existiert bereits in der Liste
			{
				return -1;
			}
			current = current -> next;
		}

		if (current -> val == value) //Value vom letzten Knoten überprüfen
			{
				return -1;
			}

		current -> next = new_node;
		return value;
	}
	return -1;
}

static int removeElement(void) {
	// TODO: implement me!
	return -1;
}

int main (int argc, char* argv[]) {
	printf("insert 47: %d\n", insertElement(47));
	printf("insert 11: %d\n", insertElement(11));
	printf("insert 23: %d\n", insertElement(23));
	printf("insert 11: %d\n", insertElement(11));

	printf("remove: %d\n", removeElement());
	printf("remove: %d\n", removeElement());

	// TODO: add more tests

	exit(EXIT_SUCCESS);
}
