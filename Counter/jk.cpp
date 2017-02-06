#include "jk.h"

JK::JK() : q(0), q_bar(1), clock(0), J(0), K(0) { }

void JK::pulse(int clk, int j, int k) {
	J = j;
	K = k;
    if(clk!=clock && clk) {
        // Edge Triggered Flip Flip - Falling Edge

        if(j & !k)
            q = 1;
        else if(k & !j)
            q = 0;
        else if(j & k)
            q = !q;

        q_bar = !q;
    }
    clock = clk;
}

void JK::clear() {
    q = 0;
    q_bar = !q_bar;
    clock = 0;
}

void JK::async_clear(int clk) {
    if(clk!=clock && !clk) {
        clear();
    }
}

void JK::sync_clear(int prev_clk, int clk) {
    if(clk!=prev_clk && !clk) {
        clear();
    }
}
