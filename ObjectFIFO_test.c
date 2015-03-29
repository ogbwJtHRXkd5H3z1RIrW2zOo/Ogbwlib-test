#include <stdio.h>
#include "Ogbwlib.h"
#include "ObjectFIFO.h"
#include "UART.h"
#define DEBUG(...) printf(__VA_ARGS__)

void printAndEmpty(ObjectFIFO fifo) {
	U16 cnt = 0;
	DEBUG("test : %d [", ObjectFIFO_getAllocatedSize(fifo));
	while(! ObjectFIFO_isEmpty(fifo)) {
		DEBUG("%s, ", (S8*)ObjectFIFO_pop(fifo));
		cnt++;
	}
	DEBUG("] - %d\r\n", cnt);
}

void printBasic(ObjectFIFO fifo) {
	U16 i;
	DEBUG("%d [", ObjectFIFO_getAllocatedSize(fifo));
	for(i=0;i<30;i++) {
		DEBUG("%c", fifo->data[i]);
	}
	DEBUG("]\r\n");
}

void printOctets(ObjectFIFO fifo) {
	U16 i;
	DEBUG("%d [", ObjectFIFO_getAllocatedSize(fifo));
	for(i=0;i<30;i++) {
		DEBUG("%3d, ", fifo->data[i]);
	}
	DEBUG("]\r\n");
}

void ObjectFIFO_test() {
	U8 ret[50];
	S16 retCnt=0;

	ObjectFIFO fifo = ObjectFIFO_new(30);
        memset(fifo->data, '.', 30);

	ret[retCnt++]= ObjectFIFO_push(fifo, 5, "abcd")?'.':'0';//  8   8
        printOctets(fifo);
	ret[retCnt++]= ObjectFIFO_push(fifo, 5, "efgh")?'.':'0';//  8   16
	printOctets(fifo);
	ret[retCnt++]= ObjectFIFO_push(fifo, 4, "ijk")?'.':'0'; //  6   22
	ret[retCnt++]= ObjectFIFO_push(fifo, 4, "mno")?'.':'0'; //  6   28
	printAndEmpty(fifo);
	DEBUG("corr : 28 [abcd, efgh, ijk, mno, ] - 4\r\n\r\n");

	ret[retCnt++]= ObjectFIFO_push(fifo, 6, "abcde")?'.':'0';	//  8   8
	ret[retCnt++]= ObjectFIFO_push(fifo, 4, "fgh")?'.':'0';		//  6   14
	ret[retCnt++]= ObjectFIFO_push(fifo, 5, "ijkl")?'.':'0';	//  8   22
	ret[retCnt++]= ObjectFIFO_push(fifo, 7, "mnopqr")?'.':'0';	//  10  --
	ret[retCnt++]= ObjectFIFO_push(fifo, 4, "qrs")?'.':'0';		//  6   28
	ret[retCnt++]= ObjectFIFO_push(fifo, 3, "tu")?'.':'0';		//  6   --
	printAndEmpty(fifo);
	DEBUG("corr : 28 [abcde, fgh, ijkl, qrs, ] - 4\r\n\r\n");

	ret[retCnt++]= ObjectFIFO_push(fifo, 5, "abcd")?'.':'0';
	ObjectFIFO_clear(fifo);
	printAndEmpty(fifo);
	DEBUG("corr : 0 [] - 0\r\n\r\n");

	//  0........1.........2.........3|
        ret[retCnt++]= ObjectFIFO_push(fifo, 6, "abcde")?'.':'0';       //  8   8
	// |ppabcde0                      |
        ret[retCnt++]= ObjectFIFO_push(fifo, 4, "fgh")?'.':'0';		//  6   14
	// |ppabcde0ppfgh0                |
        ret[retCnt++]= ObjectFIFO_push(fifo, 5, "ijkl")?'.':'0';	//  8   22
	// |ppabcde0ppfgh0ppijkl0.        |
        ret[retCnt++]= ObjectFIFO_push(fifo, 5, "mnop")?'.':'0';	//  8   30
	// |ppabcde0ppfgh0ppijkl0.ppmnop0.|
        ObjectFIFO_pop(fifo);						//  -8  22
	// |        ppfgh0ppijkl0.ppmnop0.|
        ret[retCnt++]= ObjectFIFO_push(fifo, 4, "qrs")?'.':'0';		//  6   28
	// |ppqrs0  ppfgh0ppijkl0.ppmnop0.|
        ret[retCnt++]= ObjectFIFO_push(fifo, 4, "tuv")?'.':'0';		//  6   --
	// |ppqrs0  ppfgh0ppijkl0.ppmnop0.|
        ObjectFIFO_pop(fifo);                                           //  -6  22
	// |ppqrs0        ppijkl0.ppmnop0.|
        ObjectFIFO_pop(fifo);                                           //  -8  14
	// |ppqrs0                ppmnop0.|
        ret[retCnt++]= ObjectFIFO_push(fifo, 5, "wxyz")?'.':'0';	//  8   22
	// |ppqrs0ppwxyz0.        ppmnop0.|
	ObjectFIFO_pop(fifo);                                           //  -8  14
	// |ppqrs0ppwxyz0.                |
	ObjectFIFO_pop(fifo);                                           //  -6  8
	// |      ppwxyz0.                |
	ret[retCnt++]= ObjectFIFO_push(fifo, 3, "12")?'.':'0';          //  6   14
	ret[retCnt++]= ObjectFIFO_push(fifo, 3, "34")?'.':'0';          //  6   20
	// |      ppwxyz0.pp120.pp340.    |
        ret[retCnt++]= ObjectFIFO_push(fifo, 8, "abcdefg")?'.':'0';     //  10   --
	// |      ppwxyz0.pp120.pp340.    |
                printOctets(fifo);
                DEBUG("10 [      ppwxyz0.pp120.pp340.    ]\r\n");
        ret[retCnt++]= ObjectFIFO_push(fifo, 4, "ABC")?'.':'0';         //  6   30
	// |ppABC0ppwxyz0.pp120.pp340.    |
        printAndEmpty(fifo);
        DEBUG("corr : 30 [wxyz, 12, 34, ABC, ] - 4\r\n\r\n");

	DEBUG("test : %d - {", retCnt);
	while(--retCnt>=0) {
		DEBUG("%c", ret[retCnt]);
	}
	DEBUG("}\r\n", retCnt);
	DEBUG("corr : 22 - {.0...0......0.0.......}\r\n\r\n");
}


