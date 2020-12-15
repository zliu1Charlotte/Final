#include "cosTB.h"

int sc_main(int argc, char* argv[])
{
	cosineTB* TOP = new cosineTB("cosTB_Instance");

	sc_trace_file* VCDFile;
	VCDFile = sc_create_vcd_trace_file("cos");
	sc_trace(VCDFile, TOP->xin, "xin");
	sc_trace(VCDFile, TOP->clk, "clk");
	sc_trace(VCDFile, TOP->rst, "rst");
	sc_trace(VCDFile, TOP->start, "start");
	sc_trace(VCDFile, TOP->ready, "ready");
	sc_trace(VCDFile, TOP->out, "out");

	sc_start(4000, SC_NS);
	return 0;
}