#include <systemc.h>

class put_if : virtual public sc_interface
{
public:
	virtual bool put(sc_lv<8>) = 0;
	virtual void init() = 0;
};

class get_if : virtual public sc_interface
{
public:
	virtual bool get(sc_lv<8>&) = 0;
};


