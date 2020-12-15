#include "interfaceClasses.h"

class buffer : public put_if, public get_if
{
	//bool full;
	//sc_lv<8> saved;
	//sc_event put_event, get_event;
	//sc_mutex busy;
public:
	//buffer() : full(false) {};
	//~buffer() {};
	buffer() { tos = 0; };
	bool put(sc_lv<8> data);
	void init();
	bool get(sc_lv<8>& data);

private:
	sc_lv<8> contents[5];
	int tos;
};