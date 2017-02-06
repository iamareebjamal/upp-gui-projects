#ifndef COUNTER_JKFLIPFLOP_H
#define COUNTER_JKFLIPFLOP_H

class JK {

private:
    int clock, q, q_bar;
    int J, K;

public:
    JK();

    void pulse(int clk, int j, int k);

    inline int get_data(){
        return q;
    }
    
    inline int get_data_bar(){
        return q_bar;
    }

    inline int get_data(int clk, int j, int k){
        pulse(clk, j, k);
        return q;
    }

    inline int get_clock(){
        return clock;
    }
    
    inline int get_j(){
        return J;
    }
    
    inline int get_k(){
    	return K;   
    }

    void clear();
    void async_clear(int clk);
    void sync_clear(int prev_clk, int clk);
};


#endif //COUNTER_JKFLIPFLOP_H
