#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf) {
	SortedListPtr list = malloc(sizeof(struct SortedList));
	if(list != NULL) {
		list->data = NULL;
		list->next = NULL; 
		list->comparator = cf;
		list->deleted = 1;
	}
	return list;
}

void SLDestroy(SortedListPtr list) {
	SortedListPtr temp, temp2;	/*Pointers used to parse through the list*/
	temp = temp2 = list;		/*Point the pointers to the start*/
	while(temp != NULL){		/*While you haven't reached the end of the list*/
		temp2 = temp->next;		/*Move the other pointer to the next node*/
		if(temp2 == NULL){		/*if next node doesn't exist that means you're at the end*/
			free(temp);			/*Free that node*/
			break;
		}
		else{					/*else the list still has more stuff*/
			free(temp);			/*Free the node itself*/
			temp = temp2;		/*Move onto the next node in the list*/
		}
	}	
}

int SLRemove(SortedListPtr list, void *newObj){
	SortedListPtr temp;	/*Pointer used to parse through the list*/
	int compv, successval = 0;	/*compv is the value of comparison, successval is value of removal*/
	temp = list;		/*Point the pointers to the beginning of the list*/
	if(temp == NULL)
	{
		return successval;		/*If the list is empty just return*/
	}
	else
	{					/*if not check the list*/
		while(temp != NULL)
		{
			compv = list->comparator(temp->data, newObj);
			if(compv == 0)		/*found an object to delete*/
			{
				do {	/*delete more of the same object*/
					successval = 1;
					temp->deleted = 0;	/*set delete flag*/
					if((temp = temp->next) == NULL)
						break;	/*break out of while loop if there are no more objects*/
				} while(list->comparator(temp->data, newObj) == 0);	/*checks if there are still more of the same object*/
				break;	/*no more of the same object to delete*/
			}
			temp = temp->next;
		}
		return successval;
	}
}

int SLInsert(SortedListPtr list, void *newObj) {
	SortedListPtr prev = NULL;
	SortedListPtr temp = list;
	int compval;
	if(list->data == NULL){
		list->data = newObj;
		return 1;
	}
	SortedListPtr newObject = malloc(sizeof(struct SortedList));
	if(newObject == NULL)
		return 0;
	newObject->data = newObj;
	newObject->next = NULL;
	newObject->comparator = list->comparator;
	newObject->deleted = 1;
	while(temp != NULL){
		compval = list->comparator(temp->data, newObj);
		if(compval < 0){
			if(prev == NULL){
				newObject->data = list->data;
				newObject->next = list->next;
				newObject->deleted = list->deleted;
				list->data = newObj;
				list->next = newObject;
				list->deleted = 1;
				return 1;
			}
			prev->next = newObject;
			newObject->next = temp;
			return 1;
		}
		else if(compval == 0){
			newObject->next = temp->next;
			temp->next = newObject;
			return 1;
		}
		else{
			prev = temp;
			temp = temp->next;
		}
	}
	prev->next = newObject;
	return 1;

}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
	SortedListIteratorPtr iterator = malloc(sizeof(*iterator));
	if(iterator != NULL) {
		iterator->current = list;
		iterator->previous = NULL;
	}
	return iterator;
}

void SLDestroyIterator(SortedListIteratorPtr iter) {
	free(iter);
}

void *SLNextItem(SortedListIteratorPtr iter) {
	if(iter->current == NULL)
		return NULL;
	if(iter->previous != NULL) {
		while(iter->previous->next != iter->current) {
			iter->previous = iter->previous->next;
			if(iter->previous->deleted == 1) {
				iter->current = iter->previous;
				break;
			}
		}
	}
	do {
		if(iter->current->deleted == 1)
			break;
		iter->current = iter->current->next;
	} while(iter->current != NULL);
	void *object = NULL;
	if(iter->current != NULL) {
		object = iter->current->data;
		iter->previous = iter->current;
		iter->current = iter->current->next;
	}
	return object;
}
