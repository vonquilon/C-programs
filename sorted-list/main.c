/*
 * sorted-list.c
 */

#include	<stdio.h>
#include	<string.h>
#include	"sorted-list.h"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

int main()
{
	void *dataholder,*dataholder2;	
	SortedListIteratorPtr x,y;
	SortedListPtr temp;
	SortedListPtr list = SLCreate(&compareInts); /*3 SLCreates called consecutively*/
	SortedListPtr list2 = SLCreate(&compareDoubles);
	SortedListPtr list3 = SLCreate(&compareStrings);
    SortedListPtr list4 = SLCreate(&compareInts);

	int a = 8,b = 4, c = 12, d = 32 , e = 4, z = 8;
	SLInsert(list,&a);
	x = SLCreateIterator(list);	
	SLInsert(list,&z);
	SLRemove(list,&a);
	SLInsert(list,&b);		
	SLInsert(list,&c);
	y = SLCreateIterator(list);
	SLInsert(list,&d);
	SLInsert(list,&e);
	SLRemove(list, &d);
	dataholder = SLNextItem(x);
	dataholder2 = SLNextItem(y);
	while(dataholder != NULL){
		printf("%d\n",*(int*)dataholder);
		dataholder = SLNextItem(x);
	}
	while(dataholder2 != NULL){
		printf("%d\n", *(int*)dataholder2);
		dataholder2 = SLNextItem(y);
	}
	temp = list;
	while(temp != NULL){
		printf("%d, deleted: %d\n",*(int*)temp->data, temp->deleted);
		temp = temp->next;
	}
	SLDestroy(list);
	
    SortedListIteratorPtr xx;
    void *dataholder3;
	double f = 8.56, g = 4.99, h = 12.0112, i = 32.56, j = 4.99;
	SLInsert(list2, &f);
	SLInsert(list2, &g);
	SLInsert(list2, &h);
	SLInsert(list2, &i);
	SLInsert(list2, &j);
	SLRemove(list2, &f);
	temp = list2;
    xx = SLCreateIterator(list2);
    dataholder3 = SLNextItem(xx);
    while(dataholder3 != NULL){
        printf("%f\n",*(double*)dataholder3);
        dataholder3 = SLNextItem(xx);
    }
	while(temp != NULL){
		printf("%f, deleted: %d\n",*(double*)temp->data, temp->deleted);
		temp = temp->next;
	}
	SLDestroy(list2);

	char *k = "dude"; char *l = "dude"; char *m = "breaking";
	char *n = "bad"; char *o = "walter"; char *p = "white";
	SLInsert(list3, k);
	SLInsert(list3, l);
	SLInsert(list3, m);
	SLInsert(list3, n);
	SLInsert(list3, o);
	SLInsert(list3, p);
	SLRemove(list3, k);
	temp = list3;
	while(temp != NULL){
		printf("%s, deleted: %d\n", temp->data, temp->deleted);
		temp = temp->next;
	}
	SLDestroy(list3);

    void *dataholder4;
    int aa = 7; int bb = 5; int cc = 4; int dd = 3; int ee = 2;
    SortedListIteratorPtr yy;
    SLInsert(list4, &aa);
    SLInsert(list4, &bb);
    SLInsert(list4, &dd);
    yy = SLCreateIterator(list4);
    dataholder4 = SLNextItem(yy);
    printf("%d\n",*(int*)dataholder4);
    dataholder4 = SLNextItem(yy);
    printf("%d\n",*(int*)dataholder4);
    SLRemove(list4, &bb);
    SLInsert(list4, &cc);
    SLInsert(list4, &ee);
    dataholder4 = SLNextItem(yy);
    printf("%d\n",*(int*)dataholder4);
    dataholder4 = SLNextItem(yy);
    printf("%d\n",*(int*)dataholder4);
    dataholder4 = SLNextItem(yy);
    printf("%d\n",*(int*)dataholder4);
    temp = list4;
    while(temp != NULL){
        printf("%d, deleted: %d\n",*(int*)temp->data,temp->deleted);
        temp = temp->next;
    }
	return 0;
}
