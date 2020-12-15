#include "memWriterReader.h"
#include "memoryUnit.h"

SC_MODULE(memWriterReader_TB)
{
	memWriterReader *WR1;
	memoryUnit *MU1;

	SC_CTOR(memWriterReader_TB)
	{
		WR1 = new memWriterReader("WR");
		MU1 = new memoryUnit("memory");
 		   WR1->memWRSocket.bind(MU1->memSocket);
	}
};

