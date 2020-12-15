#include "bufferChannel.h"

SC_MODULE (transmitter) {
	sc_port<put_if> out;

	SC_CTOR(transmitter) {
		SC_THREAD (putting);
	}
	void putting();
};

SC_MODULE (receiver) {
	sc_export<get_if> in;

	SC_CTOR(receiver) {
		SC_THREAD (getting);
    }
	void getting();
};
