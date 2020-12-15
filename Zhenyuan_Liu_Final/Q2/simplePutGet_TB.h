#include "simplePutGet.h"

SC_MODULE (simplePutGet_TB) {
	
	buffer* BUF1;
	transmitter* TRS1;
	receiver* RCV1; 

	SC_CTOR(simplePutGet_TB) {
		BUF1 = new buffer();
		TRS1 = new transmitter("Transmitter");
			TRS1->out(*BUF1);
		RCV1 = new receiver("Receiver");
			RCV1->in(*BUF1);
	}
};
