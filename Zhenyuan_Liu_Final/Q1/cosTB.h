#include "cos.h"

SC_MODULE(cosineTB)
{
	sc_signal<sc_logic> clk, rst, start, ready;
	sc_signal<float> xin, out;

	cosine<8, 7>* COS;

	SC_CTOR(cosineTB)
	{
		COS = new cosine<8,7>("EXP_Instance");
		(*COS) (xin, clk, rst, start, out, ready);

		SC_THREAD(resetting);
		SC_THREAD(clocking);
		SC_THREAD(inGenerating);
		SC_THREAD(displaying);
		sensitive << ready;

	}
	void resetting();
	void clocking();
	void inGenerating();
	void displaying();
};
