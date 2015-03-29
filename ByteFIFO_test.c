#include "Ogbwlib.h"
#include "ByteFIFO.h"
#include <stdio.h>
#define DEBUG(...) printf(__VA_ARGS__)
#define DEBUG_ERR(v)	DEBUG("%d ", v);


void printAndEmpty(ByteFIFO fifo) {
	U16 cnt = 0;
	DEBUG("test : [");
	while(! ByteFIFO_isEmpty(fifo)) {
		U8 tmp = ByteFIFO_get(fifo), tmp2 = ByteFIFO_pop(fifo);
		if(tmp==tmp2) {
			DEBUG("%c, ", tmp2);
		} else {
			DEBUG("(%c%c)", tmp, tmp2);
		}
		cnt++;
	}
	DEBUG("] - %d\r\n", cnt);
}

void ByteFIFO_test() {
	ByteFIFO fifo = ByteFIFO_new(12);
	// ByteFIFO_new			erreur de réglage révélé dans la suite

	ByteFIFO_push(fifo, 'a');
	ByteFIFO_push(fifo, 'b');
	ByteFIFO_push(fifo, 'c');
	ByteFIFO_push(fifo, 'd');
	ByteFIFO_push(fifo, 'e');
	printAndEmpty(fifo);
	DEBUG("corr : [a, b, c, d, e, ] - 5\r\n\r\n");
	// ByteFIFO_isEmpty
	// ByteFIFO_pop			sauf contrôle de pile vide
	//				sauf bon positionnement du buffer après
	// ByteFIFO_push		sauf contrôle de pile pleine

	U8 test = 'a';
	while(!ByteFIFO_isFull(fifo)) {
		ByteFIFO_push(fifo, test++);
		DEBUG_ERR(ByteFIFO_getAvailableSize(fifo));
		DEBUG_ERR(ByteFIFO_getDataSize(fifo));
	}
	DEBUG("\r\n11 1 10 2 9 3 8 4 7 5 6 6 5 7 4 8 3 9 2 10 1 11 0 12 \r\n");
	printAndEmpty(fifo);
	DEBUG("corr : [a, b, c, d, e, f, g, h, i, j, k, l, ] - 12\r\n\r\n");
	// ByteFIFO_get
	// ByteFIFO_isFull
	// ByteFIFO_getDataSize
	// ByteFIFO_getAvailableSize

	test = 'a';
	while(ByteFIFO_push(fifo, test++)==ByteFIFO_ok);
	printAndEmpty(fifo);
	DEBUG("corr : [a, b, c, d, e, f, g, h, i, j, k, l, ] - 12\r\n\r\n");
	// ByteFIFO_push		contrôle de pile pleine

	DEBUG_ERR(ByteFIFO_pop(fifo));
	DEBUG_ERR(ByteFIFO_pop(fifo));
	DEBUG_ERR(ByteFIFO_pop(fifo));
	DEBUG_ERR(ByteFIFO_pop(fifo));
	printAndEmpty(fifo);
	DEBUG("0 0 0 0 corr : [] - 0\r\n\r\n");
	// ByteFIFO_pop			contrôle de pile vide, la suite bug si mal géré

	DEBUG_ERR(ByteFIFO_pushBlock(fifo, 5, "abcde"));
	printAndEmpty(fifo);
	DEBUG("0 corr : [a, b, c, d, e, ] - 5\r\n\r\n");
	// ByteFIFO_pushBlock	sauf contrôle de pile pleine

	DEBUG_ERR(ByteFIFO_pushBlock(fifo, 5, "abcde"));
	ByteFIFO_clear(fifo);
	printAndEmpty(fifo);
	DEBUG("0 corr : [] - 0\r\n\r\n");
	// inline void ByteFIFO_clear(ByteFIFO fifo);

	DEBUG_ERR(ByteFIFO_pushBlock(fifo, 5, "abcde"));
	DEBUG_ERR(ByteFIFO_pushBlock(fifo, 5, "12345"));
	DEBUG_ERR(ByteFIFO_pushBlock(fifo, 5, "abcde"));
	printAndEmpty(fifo);
	DEBUG("0 0 1 corr : [a, b, c, d, e, 1, 2, 3, 4, 5, ] - 10\r\n\r\n");
	// ByteFIFO_pushBlock	contrôle de pile pleine

	DEBUG("%d ", ByteFIFO_pushBlock(fifo, 5, "abcde"));
	DEBUG("%d ", ByteFIFO_push(fifo, 'k'));
	DEBUG("%d ", ByteFIFO_pushBlock(fifo, 5, "12345"));
	DEBUG("%c ", ByteFIFO_pop(fifo));
	DEBUG("%c ", ByteFIFO_pop(fifo));
	DEBUG("%c ", ByteFIFO_pop(fifo));
	DEBUG("%c ", ByteFIFO_pop(fifo));
	DEBUG("%d ", ByteFIFO_pushBlock(fifo, 5, "zyxwv"));
	DEBUG("%d ", ByteFIFO_pushBlock(fifo, 4, "hjtd"));
	printAndEmpty(fifo);
	DEBUG("0 0 0 a b c d 0 1 corr : [e, k, 1, 2, 3, 4, 5, z, y, x, w, v, ] - 12\r\n\r\n");
	// ByteFIFO_pop			bon positionnement du buffer après

	U8 popOutput[10];

	ByteFIFO_pushBlock(fifo, 5, "abcde");	//abcde
	ByteFIFO_push(fifo, 'k');				//abcdek
	ByteFIFO_pushBlock(fifo, 5, "12345");	//abcdek12345
	ByteFIFO_popBlock(fifo, 3, popOutput);	//dek12345
	DEBUG("%.3s-", popOutput);	//abc
	ByteFIFO_popBlock(fifo, 3, popOutput);	//12345
	DEBUG("%.3s-", popOutput);	//dek
	ByteFIFO_pushBlock(fifo, 5, "vwxyz");	//12345vwxyz
	ByteFIFO_popBlock(fifo, 5, popOutput);	//vwxyz
	DEBUG("%.5s-", popOutput);	//12345
	DEBUG("\r\nabc-dek-12345-\r\n");
	printAndEmpty(fifo);
	DEBUG("corr : [v, w, x, y, z, ] - 5\r\n\r\n");
	// ByteFIFO_popBlock			sauf contrôle de début de pile

	U16 cnt=8;

	ByteFIFO_pushBlock(fifo, 5, "abcde");	//abcde
	ByteFIFO_push(fifo, 'k');				//abcdek
	ByteFIFO_pushBlock(fifo, 5, "12345");	//abcdek12345
	while(!ByteFIFO_isEmpty(fifo) && cnt!=0) {
		ByteFIFO_Error err = ByteFIFO_popBlock(fifo, cnt, popOutput);
		if(err == ByteFIFO_ok) {
			DEBUG("%.*s(%u)-", cnt, popOutput, cnt);
		} else {
			cnt--;
		}
	}
	DEBUG("\r\nabcdek12(8)-345(3)-\r\n");
	printAndEmpty(fifo);
	DEBUG("corr : [] - 0\r\n\r\n");
	// ByteFIFO_popBlock			contrôle de début de pile

	ByteFIFO_free(fifo);
	// inline void ByteFIFO_free(ByteFIFO fifo);
}
