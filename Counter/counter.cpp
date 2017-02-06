#include "counter.h"

Counter::Counter(): clk(0), prev_clk(0), clr(false) { }

void Counter::next_cycle(int clock) {
    clk = clock;
	clr = false;

    a.pulse(clk, 1, 1);
    b.pulse(a.get_data(), 1, 1);
    c.pulse(b.get_data(), 1, 1);
    d.pulse(c.get_data(), 1, 1);

    prev_clk = clk;
}