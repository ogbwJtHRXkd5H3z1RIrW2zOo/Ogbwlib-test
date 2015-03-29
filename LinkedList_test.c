#include <stdio.h>
#include <stdlib.h>
#include "Ogbwlib.h"
#include "algos/LinkedList.h"
#include "modules/uart/basicUART1.h"



U8 isSmaller(void* a, void* b) {
    return *(char*)a < *(char*)b;
}

void printPtr(LinkedList l) {
    UART1_printf("[");
    LinkedList_item * curr = l.first;
    while(curr!=null) {
        UART1_printf("(%p)%p(%p), ",curr->prev, curr, curr->next);
        curr = curr->next;
    }
    UART1_printf("] %d\r\n", LinkedList_size(l));
}

void checkPtr(LinkedList l) {
    LinkedList_item * prev = l.first->prev;
    LinkedList_item * curr = l.first;

    if(prev != null) {
        UART1_printf("first->prev != null\r\n");
    }
    while(curr!=null) {
        if(curr->prev != prev) {
            UART1_printf("curr->prev != prev =>");
            printPtr(l);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void print(LinkedList l) {
    checkPtr(l);
    
    UART1_printf("[");
    LinkedList_item * curr = l.first;
    while(curr!=null) {
        UART1_printf("%c, ",*(char*)curr->data);
        curr = curr->next;
    }
    UART1_printf("] %d\r\n", LinkedList_size(l));

}

U8 voyelle(void* data) {
    char d = *(char*) data;
    return d=='a' || d=='e' || d=='i' || d=='o' || d=='u' || d=='y';
}

U8 consonne(void* data) {
    char d = *(char*) data;
    return d!='a' && d!='e' && d!='i' && d!='o' && d!='u' && d!='y';
}

void todo(void *data) {
    UART1_printf("coucou %c\r\n", *(char*)data);
}



char* data="aBcDeFgHiJkLmNoPqRsTuVwXyZ";
void LinkedList_test(void) {
    

    LinkedList l = LinkedList_new();

    UART1_printf("test new \r\n");
    UART1_printf("t [] 0\r\n");
    UART1_printf("%c ", LinkedList_isEmpty(l)?'t':'f');
    print(l);

    UART1_printf("\r\ntest addFirst\r\n");
    LinkedList_addFirst(&l,data);
    LinkedList_addFirst(&l,data+1);
    LinkedList_addFirst(&l,data+2);
    LinkedList_addFirst(&l,data+3);
    UART1_printf("f [D, c, B, a, ] 4\r\n");
    UART1_printf("%c ", LinkedList_isEmpty(l)?'t':'f');
    print(l);

    UART1_printf("\r\ntest addLast\r\n");
    LinkedList_addLast(&l, data+4);
    LinkedList_addLast(&l, data+5);
    LinkedList_addLast(&l, data+6);
    LinkedList_addLast(&l, data+7);
    UART1_printf("[D, c, B, a, e, F, g, H, ] 8\r\n");
    print(l);

    UART1_printf("\r\ntest addMiddle\r\n");
    LinkedList_addMiddle(&l, data+8, 0);
    LinkedList_addMiddle(&l, data+9, 9);
    LinkedList_addMiddle(&l, data+10, 5);
    LinkedList_addMiddle(&l, data+11, 3);
    UART1_printf("[i, D, c, L, B, a, k, e, F, g, H, J, ] 12\r\n");
    print(l);

    UART1_printf("\r\ntest get\r\n");
    UART1_printf("i J i J k D i\r\n");
    char tmp1[] = {*(char*)LinkedList_getFirst(l),
            *(char*)LinkedList_getLast(l),
            *(char*)LinkedList_getMiddle(l,0),
            *(char*)LinkedList_getMiddle(l,11),
            *(char*)LinkedList_getMiddle(l,6),
            *(char*)LinkedList_getFilter(l, consonne),
            *(char*)LinkedList_getFilter(l, voyelle),0};
    UART1_printf("%c %c %c %c %c %c \r\n", tmp1[0], tmp1[4], tmp1[2], tmp1[3], tmp1[4], tmp1[5]);

    UART1_printf("\r\ntest removeMiddle\r\n");
    UART1_printf("L k J i - [D, c, B, a, e, F, g, H, ] 8\r\n");
    char tmp2[] = { *(char*)LinkedList_removeMiddle(&l, 3),
             *(char*)LinkedList_removeMiddle(&l, 5),
             *(char*)LinkedList_removeMiddle(&l, 9),
             *(char*)LinkedList_removeMiddle(&l, 0), 0};
    UART1_printf("%c %c %c %c - ", tmp2[0], tmp2[1], tmp2[2], tmp2[3]);
    print(l);

    UART1_printf("\r\ntest removeFirst\r\n");
    UART1_printf("D c B a - [e, F, g, H, ] 4\r\n");
    char tmp3[]={*(char*)LinkedList_removeFirst(&l),
             *(char*)LinkedList_removeFirst(&l),
             *(char*)LinkedList_removeFirst(&l),
             *(char*)LinkedList_removeFirst(&l), 0};
    UART1_printf("%s - ", tmp3);
    print(l);

    UART1_printf("\r\ntest removeLast\r\n");
    UART1_printf("H g F - [e, ] 1\r\n");
    char tmp4[] = {*(char*)LinkedList_removeLast(&l),
             *(char*)LinkedList_removeLast(&l),
             *(char*)LinkedList_removeLast(&l),0};
    UART1_printf("%c %c %c  - ", tmp4[0], tmp4[1], tmp4[2]);
    print(l);

    UART1_printf("\r\ntest addSorted\r\n");
    LinkedList_addSorted(&l,data, isSmaller);
    LinkedList_addSorted(&l,data+1, isSmaller);
    LinkedList_addSorted(&l,data+2, isSmaller);
    LinkedList_addSorted(&l,data+3, isSmaller);
    UART1_printf("[B, D, a, c, e, ] 5\r\n");
    print(l);

    UART1_printf("\r\ntest reverse\r\n");
    LinkedList_reverse(&l);
    UART1_printf("[e, c, a, D, B, ] 5\r\n");
    print(l);

    UART1_printf("\r\ntest addAllFirst\r\n");
    LinkedList l2 = LinkedList_new();
    LinkedList_addFirst(&l2, data+25);
    LinkedList_addAllFirst(&l2, l);
    UART1_printf("[e, c, a, D, B, Z, ] 6\r\n");
    print(l2);

    UART1_printf("\r\ntest addAllLast\r\n");
    l2 = LinkedList_new();
    LinkedList_addFirst(&l2, data+25);
    LinkedList_addAllLast(&l2, l);
    UART1_printf("[Z, e, c, a, D, B, ] 6\r\n");
    print(l2);
   
    UART1_printf("\r\ntest addAllMiddle\r\n");
    l2 = LinkedList_new();
    LinkedList_addFirst(&l2, data+25);
    LinkedList_addFirst(&l2, data+24);
    LinkedList_addAllMiddle(&l2, l, 1);
    UART1_printf("[y, e, c, a, D, B, Z, ] 7\r\n");
    print(l2);

    UART1_printf("\r\ntest addAllSorted\r\n");
    l2 = LinkedList_new();
    LinkedList_addFirst(&l2, data+25);
    LinkedList_addAllSorted(&l2, l, isSmaller);
    UART1_printf("[B, D, Z, a, c, e, ] 6\r\n");
    print(l2);

    UART1_printf("\r\ntest merge\r\n");
    l=LinkedList_merge(l,l2);
    UART1_printf("[e, c, a, D, B, B, D, Z, a, c, e, ] 11\r\n");
    print(l);

    UART1_printf("\r\ntest removePtr\r\n");
    UART1_printf("c - [e, a, D, B, B, D, Z, a, c, e, ] 10\r\n");
    UART1_printf("%c - ", LinkedList_removePtr(&l, data+2));
    print(l);

    UART1_printf("\r\ntest removeAllPtr\r\n");
    UART1_printf("2 - [e, a, B, B, Z, a, c, e, ] 8\r\n");
    UART1_printf("%d - ", LinkedList_removeAllPtr(&l, data+3));
    print(l);

    UART1_printf("\r\ntest removeFilter\r\n");
    UART1_printf("B - [e, a, B, Z, a, c, e, ] 7\r\n");
    UART1_printf("%c - ", LinkedList_removeFilter(&l, consonne));
    print(l);

    UART1_printf("\r\ntest removeIf\r\n");
    UART1_printf("4 - [B, Z, c, ] 3\r\n");
    UART1_printf("%d - ", LinkedList_removeIf(&l, voyelle));
    print(l);

    UART1_printf("\r\ntest removeAll\r\n");
    UART1_printf("3 - [] 0\r\n");
    UART1_printf("%d - ", LinkedList_removeAll(&l));
    print(l);

    UART1_printf("\r\ntest mergeSorted");
    l = LinkedList_new();
    LinkedList_addSorted(&l, data+14, isSmaller);
    LinkedList_addSorted(&l, data+16, isSmaller);
    LinkedList_addSorted(&l, data+23, isSmaller);
    LinkedList_addSorted(&l, data+5, isSmaller);
    l2 = LinkedList_new();
    LinkedList_addSorted(&l2, data+18, isSmaller);
    LinkedList_addSorted(&l2, data+21, isSmaller);
    LinkedList_addSorted(&l2, data+15, isSmaller);
    LinkedList_addSorted(&l2, data+8, isSmaller);
    LinkedList_addSorted(&l2, data+20, isSmaller);
    print(l);
    print(l2);
    UART1_printf("[F, P, V, X, i, o, q, s, u, ] 9\r\n");
    l=LinkedList_mergeSorted(l2,l,isSmaller);
    print(l);
    
    l = LinkedList_new();
    LinkedList_addSorted(&l, data+14, isSmaller);
    LinkedList_addSorted(&l, data+16, isSmaller);
    LinkedList_addSorted(&l, data+23, isSmaller);
    LinkedList_addSorted(&l, data+5, isSmaller);
    l2 = LinkedList_new();
    LinkedList_addSorted(&l2, data+18, isSmaller);
    LinkedList_addSorted(&l2, data+21, isSmaller);
    LinkedList_addSorted(&l2, data+15, isSmaller);
    LinkedList_addSorted(&l2, data+8, isSmaller);
    LinkedList_addSorted(&l2, data+20, isSmaller);
    l=LinkedList_mergeSorted(l,l2,isSmaller);
    print(l);

    UART1_printf("\r\ntest executeAll\r\n");
    LinkedList_executeAll(l,todo);

    UART1_printf("\r\ntest executeIf\r\n");
    LinkedList_executeIf(l, voyelle, todo);

    UART1_printf("\r\ntest reverseExecuteAll\r\n");
    LinkedList_reverseExecuteAll(l, todo);

    UART1_printf("\r\ntest reverseExecuteIf\r\n");
    LinkedList_reverseExecuteIf(l, consonne, todo);

}

/*
void LinkedList_deleteFirst(volatile LinkedList* list);
void LinkedList_deleteMiddle(volatile LinkedList* list, U16 num);
void LinkedList_deleteLast(volatile LinkedList* list);
void LinkedList_deletePtr(volatile LinkedList* list, void* data);
void LinkedList_deleteFilter(volatile LinkedList* list, U8 (*match)(void*));
U16 LinkedList_deleteAll(volatile LinkedList* list);
U16 LinkedList_deleteAllWithDuplicates(volatile LinkedList* list);
U16 LinkedList_deleteAllPtr(volatile LinkedList* list, void* data);
U16 LinkedList_deleteIf(volatile LinkedList* list, U8 (*match)(void*));
*/
