#include <stdio.h>
#include "Ogbwlib.h"
#include "ObjectLIFO.h"
#define DEBUG(...) printf(__VA_ARGS__)
#define DEBUG2(...) DEBUG(__VA_ARGS__)
#define DEBUG4(...) 

void printAndEmpty(ObjectLIFO lifo) {
	U16 cnt = 0;
	DEBUG("test : %d [", lifo->allocatedSize);
	while(! ObjectLIFO_isEmpty(lifo)) {
		DEBUG("%s, ", (S8*)ObjectLIFO_pop(lifo));
		cnt++;
	}
	DEBUG("] - %d\r\n", cnt);
}

void printBasic(ObjectLIFO lifo) {
	U16 i;
	DEBUG("%d [", lifo->allocatedSize);
	for(i=0;i<30;i++) {
		DEBUG("%c, ", lifo->data[i]);
	}
	DEBUG("]\r\n");
}

void ObjectLIFO_test() {
	U8 ret[50];
	S16 retCnt=0;

	ObjectLIFO lifo = ObjectLIFO_new(30);

	ret[retCnt++]= ObjectLIFO_push(lifo, 5, "abcd")?'.':'0';	//8	8		10	10
	ret[retCnt++]= ObjectLIFO_push(lifo, 5, "efgh")?'.':'0';	//8	16		10	20
	ret[retCnt++]= ObjectLIFO_push(lifo, 4, "ijk")?'.':'0';		//6	22		8	28
	ret[retCnt++]= ObjectLIFO_push(lifo, 4, "mno")?'.':'0';		//6	28		8	--
	printAndEmpty(lifo);
	DEBUG2("corr : 28 [mno, ijk, efgh, abcd, ] - 4\r\n\r\n");
	DEBUG4("corr : 28 [ijk, efgh, abcd, ] - 3\r\n\r\n");

	ret[retCnt++]= ObjectLIFO_push(lifo, 6, "abcde")?'.':'0';	//8	8		10	10
	ret[retCnt++]= ObjectLIFO_push(lifo, 4, "fgh")?'.':'0';		//6	14		8	18
	ret[retCnt++]= ObjectLIFO_push(lifo, 5, "ijkl")?'.':'0';	//8	22		10	28
	ret[retCnt++]= ObjectLIFO_push(lifo, 7, "mnopqr")?'.':'0';	//10--		12	--
	ret[retCnt++]= ObjectLIFO_push(lifo, 4, "qrs")?'.':'0';		//6	28		8	--
	ret[retCnt++]= ObjectLIFO_push(lifo, 3, "tu")?'.':'0';		//6	--		8	--
	printAndEmpty(lifo);
	DEBUG2("corr : 28 [qrs, ijkl, fgh, abcde, ] - 4\r\n\r\n");
	DEBUG4("corr : 28 [ijkl, fgh, abcde, ] - 3\r\n\r\n");

	ret[retCnt++]= ObjectLIFO_push(lifo, 5, "abcd")?'.':'0';
	ObjectLIFO_clear(lifo);
	printAndEmpty(lifo);
	DEBUG("corr : 0 [] - 0\r\n\r\n");

	ret[retCnt++]= ObjectLIFO_push(lifo, 6, "abcde")?'.':'0';	//8	8		10	10
	ret[retCnt++]= ObjectLIFO_push(lifo, 4, "fgh")?'.':'0';		//6	14		8	18
	ret[retCnt++]= ObjectLIFO_push(lifo, 5, "ijkl")?'.':'0';	//8	22		10	28
	ret[retCnt++]= ObjectLIFO_push(lifo, 5, "mnop")?'.':'0';	//8	30		10	--
	ObjectLIFO_pop(lifo);						//-8 22		-10	18
	ret[retCnt++]= ObjectLIFO_push(lifo, 4, "qrs")?'.':'0';		//6	28		8	26
	ret[retCnt++]= ObjectLIFO_push(lifo, 4, "tuv")?'.':'0';		//6	--		8	--
	ObjectLIFO_pop(lifo);                                           //-6 22		-8	18
	ObjectLIFO_pop(lifo);                                           //-8 14		-8	10
	ret[retCnt++]= ObjectLIFO_push(lifo, 5, "wxyz")?'.':'0';	//8	22		10	20
	printAndEmpty(lifo);
	DEBUG2("corr : 22 [wxyz, fgh, abcde, ] - 3\r\n\r\n");
	DEBUG4("corr : 20 [wxyz, abcde, ] - 2\r\n\r\n");

	DEBUG("test : %d - {", retCnt);
	while(--retCnt>=0) {
		DEBUG("%c", ret[retCnt]);
	}
	DEBUG("}\r\n", retCnt);
	DEBUG2("corr : 17 - {.0......0.0.......}\r\n\r\n");
	DEBUG4("corr : 17 - {.0.0....000...0...}\r\n\r\n");
}