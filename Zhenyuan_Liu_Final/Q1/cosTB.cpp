#include "cosTB.h"

void cosineTB::resetting()
{
	while (true)
	{
		wait(7, SC_NS);
		rst = SC_LOGIC_0;
		wait(11, SC_NS);
		rst = SC_LOGIC_1;
		wait(58, SC_NS);
		rst = SC_LOGIC_0;
		wait();
	}
}

void cosineTB::clocking()
{
	while (true)
	{
		wait(17, SC_NS);
		clk = SC_LOGIC_0;
		wait(17, SC_NS);
		clk = SC_LOGIC_1;
	}
}

void cosineTB::inGenerating()
{
	while (true)
	{
		xin = 0.5;
		start = SC_LOGIC_0;
		wait(600, SC_NS);

		xin = 1.5;
		start = SC_LOGIC_1;
		wait(600, SC_NS);

		xin = 1;
		wait(600, SC_NS);

		xin = 1.3;
		wait(600, SC_NS);

		xin = 0.3;
		wait(600, SC_NS);

		xin = 0.5;
		wait(600, SC_NS);

		wait();
	}
}

void cosineTB::displaying()
{
	while (true) {
		if (ready == '1')
			cout << " cos " << xin << " = " << out << " - Time : " << sc_time_stamp() << endl;
		wait();
	}
}

