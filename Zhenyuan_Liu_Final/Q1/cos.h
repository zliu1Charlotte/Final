#include <iostream>
#include <systemc.h>

template <int NumClk, int N>
SC_MODULE(cosine)
{
	sc_in<float> xin;
	sc_in<sc_logic> clk;
	sc_in<sc_logic> rst;
	sc_in<sc_logic> start;

	sc_out<float> out;
	sc_out<sc_logic> ready;

	sc_signal<float> xin_reg;

	SC_CTOR(cosine)
	{
		SC_THREAD(operation)
			sensitive << clk << rst;
	}
	void operation();
};


template <int NumClk, int N>
void cosine<NumClk, N>::operation()
{
	while (true)
	{
		float term = 1;
		float exp = 1;

		if (rst == '1')
		{
			ready = SC_LOGIC_0;
			out = 0;
		}

		else if ((clk == '1') && (clk->event()))
		{
			if (start == '1')
			{
				ready = SC_LOGIC_0;
				out = 0;

				xin_reg = xin->read();
				wait(clk->posedge_event());

				for (int i = 1; i < N; i++) {
					term = term * xin_reg.read() * -1 * xin_reg.read() * (1 / float(2*i)) * (1 / float(2 * i - 1));
					exp = exp + term;
				}

				for (int i = 0; i < NumClk - 1; i++)
					wait(clk->posedge_event());

				ready = SC_LOGIC_1;
				out = exp;
			}
		}
		wait();
	}
}
