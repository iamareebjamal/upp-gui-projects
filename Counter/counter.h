#ifndef COUNTER_COUNTER_H
#define COUNTER_COUNTER_H

#include "jk.h"

class Counter {
private:
    bool clk, prev_clk;
    bool clr;
    
public:
    JK a, b, c, d;

    Counter();

    void next_cycle(int clock);

    inline void next_cycle() { // Alternating clocks
        clk=!clk;
        next_cycle(clk);
    }

    inline bool get_clock() {
        return clk;
    }
    
    inline bool get_clear(){
        return clr;
    }
    
    inline int get_value(){
    	return d.get_data()*8+ c.get_data()*4 + b.get_data()*2 + a.get_data();
    }

};


#endif //COUNTER_COUNTER_H
