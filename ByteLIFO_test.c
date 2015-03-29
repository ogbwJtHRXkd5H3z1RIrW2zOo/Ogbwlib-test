#include "Ogbwlib.h"
#include "ByteLIFO.h"
#include <stdio.h>
#define DEBUG(...) printf(__VA_ARGS__)
#define DEBUG_ERR(v)	DEBUG("%d ", v);

void printAndEmpty(ByteLIFO lifo) {
	U16 cnt = 0;
	DEBUG("test : [");
	while(! ByteLIFO_isEmpty(lifo)) {
		DEBUG("%c, ", ByteLIFO_pop(lifo));
		cnt++;
	}
	DEBUG("] - %d\r\n", cnt);
}

void ByteLIFO_test() {
	ByteLIFO lifo = ByteLIFO_new(12);
	// ByteLIFO_new			erreur de réglage révélé dans la suite

	ByteLIFO_push(lifo, 'a');
	ByteLIFO_push(lifo, 'b');
	ByteLIFO_push(lifo, 'c');
	ByteLIFO_push(lifo, 'd');
	ByteLIFO_push(lifo, 'e');
	printAndEmpty(lifo);
	DEBUG("corr : [e, d, c, b, a, ] - 5\r\n\r\n");
	// ByteLIFO_isEmpty
	// ByteLIFO_pop			sauf contrôle de pile vide
	//						sauf bon positionnement du buffer après
	// ByteLIFO_push		sauf contrôle de pile pleine

	U8 test = 'a';
	while(!ByteLIFO_isFull(lifo)) {
		ByteLIFO_push(lifo, test++);
		DEBUG_ERR(ByteLIFO_getAvailableSize(lifo));
		DEBUG_ERR(ByteLIFO_getDataSize(lifo));
		if(ByteLIFO_get(lifo)!=test-1) {
			DEBUG("RATE");
		}
	}
	DEBUG("\r\n11 1 10 2 9 3 8 4 7 5 6 6 5 7 4 8 3 9 2 10 1 11 0 12 \r\n");
	printAndEmpty(lifo);
	DEBUG("corr : [l, k, j, i, h, g, f, e, d, c, b, a, ] - 12\r\n\r\n");
	// ByteLIFO_get
	// ByteLIFO_isFull
	// ByteLIFO_getDataSize
	// ByteLIFO_getAvailableSize

	test = 'a';
	while(ByteLIFO_push(lifo, test++)==ByteLIFO_ok);
	printAndEmpty(lifo);
	DEBUG("corr : [l, k, j, i, h, g, f, e, d, c, b, a, ] - 12\r\n\r\n");
	// ByteLIFO_push		contrôle de pile pleine

	DEBUG_ERR(ByteLIFO_pop(lifo));
	DEBUG_ERR(ByteLIFO_pop(lifo));
	DEBUG_ERR(ByteLIFO_pop(lifo));
	DEBUG_ERR(ByteLIFO_pop(lifo));
	printAndEmpty(lifo);
	DEBUG("0 0 0 0 corr : [] - 0\r\n\r\n");
	// ByteLIFO_pop			contrôle de pile vide, la suite bug si mal géré

	DEBUG_ERR(ByteLIFO_pushBlock(lifo, 5, "abcde"));
	printAndEmpty(lifo);
	DEBUG("0 corr : [e, d, c, b, a, ] - 5\r\n\r\n");
	// ByteLIFO_pushBlock	sauf contrôle de pile pleine

	DEBUG_ERR(ByteLIFO_rpushBlock(lifo, 5, "abcde"));
	printAndEmpty(lifo);
	DEBUG("0 corr : [a, b, c, d, e, ] - 5\r\n\r\n");
	// ByteLIFO_rpushBlock	sauf contrôle de pile pleine

	DEBUG_ERR(ByteLIFO_pushBlock(lifo, 5, "abcde"));
	ByteLIFO_clear(lifo);
	printAndEmpty(lifo);
	DEBUG("0 corr : [] - 0\r\n\r\n");
	// inline void ByteLIFO_clear(ByteLIFO lifo);

	DEBUG_ERR(ByteLIFO_pushBlock(lifo, 5, "abcde"));
	DEBUG_ERR(ByteLIFO_rpushBlock(lifo, 5, "abcde"));
	DEBUG_ERR(ByteLIFO_pushBlock(lifo, 5, "abcde"));
	DEBUG_ERR(ByteLIFO_rpushBlock(lifo, 5, "abcde"));
	printAndEmpty(lifo);
	DEBUG("0 0 1 1 corr : [a, b, c, d, e, e, d, c, b, a, ] - 10\r\n\r\n");
	// ByteLIFO_pushBlock	contrôle de pile pleine
	// ByteLIFO_rpushBlock	contrôle de pile pleine

	DEBUG_ERR(ByteLIFO_pushBlock(lifo, 5, "abcde"));
	DEBUG_ERR(ByteLIFO_push(lifo, 'k'));
	DEBUG_ERR(ByteLIFO_rpushBlock(lifo, 5, "12345"));
	ByteLIFO_pop(lifo);
	ByteLIFO_pop(lifo);
	ByteLIFO_pop(lifo);
	ByteLIFO_pop(lifo);
	DEBUG_ERR(ByteLIFO_pushBlock(lifo, 5, "zyxwv"));
	DEBUG_ERR(ByteLIFO_pushBlock(lifo, 4, "hjtd"));
	printAndEmpty(lifo);
	DEBUG("0 0 0 0 1 corr : [v, w, x, y, z, 5, k, e, d, c, b, a, ] - 12\r\n\r\n");
	// ByteLIFO_pop			bon positionnement du buffer après

	U8 popOutput[10];

	ByteLIFO_pushBlock(lifo, 5, "abcde");	//abcde
	ByteLIFO_push(lifo, 'k');				//abcdek
	ByteLIFO_rpushBlock(lifo, 5, "12345");	//abcdek54321
	ByteLIFO_popBlock(lifo, 3, popOutput);	//abcdek54
	DEBUG("%.3s-", popOutput);	//123
	ByteLIFO_rpopBlock(lifo, 3, popOutput);	//abcde
	DEBUG("%.3s-", popOutput);	//k54
	ByteLIFO_pushBlock(lifo, 5, "vwxyz");	//abcdevwxyz
	ByteLIFO_rpopBlock(lifo, 3, popOutput);	//abcdevw
	DEBUG("%.3s-", popOutput);	//xyz
	ByteLIFO_popBlock(lifo, 5, popOutput);	//ab
	DEBUG("%.5s-", popOutput);	//wvedc
	DEBUG("\r\n123-k54-xyz-wvedc-\r\n");
	printAndEmpty(lifo);
	DEBUG("corr : [b, a, ] - 2\r\n\r\n");
	// ByteLIFO_popBlock			sauf contrôle de début de pile
	// ByteLIFO_rpopBlock			sauf contrôle de début de pile

	U16 cnt=8;

	ByteLIFO_pushBlock(lifo, 5, "abcde");	//abcde
	ByteLIFO_push(lifo, 'k');				//abcdek
	ByteLIFO_rpushBlock(lifo, 5, "12345");	//abcdek54321
	while(!ByteLIFO_isEmpty(lifo) && cnt!=0) {
		ByteLIFO_Error err = ByteLIFO_popBlock(lifo, cnt, popOutput);
		if(err == ByteLIFO_ok) {
			DEBUG("%.*s(%u)-", cnt, popOutput, cnt);
		} else {
			cnt--;
		}
	}
	DEBUG("\r\n12345ked(8)-cba(3)-\r\n");
	printAndEmpty(lifo);
	DEBUG("corr : [] - 0\r\n\r\n");
	// ByteLIFO_popBlock			contrôle de début de pile


	cnt=8;

	ByteLIFO_pushBlock(lifo, 5, "abcde");	//abcde
	ByteLIFO_push(lifo, 'k');				//abcdek
	ByteLIFO_rpushBlock(lifo, 5, "12345");	//abcdek54321
	while(!ByteLIFO_isEmpty(lifo) && cnt!=0) {
		ByteLIFO_Error err = ByteLIFO_rpopBlock(lifo, cnt, popOutput);
		if(err == ByteLIFO_ok) {
			DEBUG("%.*s(%u)-", cnt, popOutput, cnt);
		} else {
			cnt--;
		}
	}
	DEBUG("\r\ndek54321(8)-abc(3)-\r\n");
	printAndEmpty(lifo);
	DEBUG("corr : [] - 0\r\n\r\n");
	// ByteLIFO_rpopBlock			contrôle de début de pile

	ByteLIFO_free(lifo);
	// inline void ByteLIFO_free(ByteLIFO lifo);
}
