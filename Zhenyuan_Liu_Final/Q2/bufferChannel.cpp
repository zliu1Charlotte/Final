#include "bufferChannel.h"

bool buffer::put(sc_lv<8> data) {
	if (tos < 5) {
		//if (full == true) wait(get_event);
		contents[tos++] = data;
		return true;
		//full = true;
		//put_event.notify();
	}return false;
}

void buffer::init() {
	tos = 0;
}

bool buffer::get(sc_lv<8>& data) {
	if (tos > 0) {
		data = contents[--tos];
		return true;
	}return false;
	//if (full == false) wait(put_event);
	//data = saved;
	//full = false;
	//get_event.notify();
}
