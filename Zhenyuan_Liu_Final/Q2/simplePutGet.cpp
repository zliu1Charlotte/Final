#include "simplePutGet.h"

void transmitter::putting() {
	//int i;
	int i = 0;
	sc_lv<8> data;
	for (i=0; i<27; i++)
	{
		data = (sc_lv<8>) i;
		wait(3, SC_NS);
		if (out->put((sc_lv<8>) (i)))
			cout << "Data: (" << data << ") was transmitted at: "
				<< sc_time_stamp() << '\n';
		else 
			cout << "Data: (" << data << ") was lost transmitting at: "
			<< sc_time_stamp() << '\n';
	}
}

void receiver::getting() {
	sc_lv<8> data;
	int i = 0; 
	for (i = 0; i < 27; i++)
	{
		wait(7, SC_NS);
		if (in->get(data))
			cout << "Data: (" << data << ") was received at: "
			<< sc_time_stamp() << '\n';
		else
			cout << "Data: (" << data << ") was lost receiving at: "
			<< sc_time_stamp() << '\n';
	}
}
