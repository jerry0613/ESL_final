#ifndef CORDIC_H_
#define CORDIC_H_

const int X0 = 9949;

#include <systemc>
using namespace std;
using namespace sc_core;
using namespace sc_dt;

class CORDIC: public sc_module {
public:
	sc_fifo<sc_int<8>> i_theta;
  	sc_fifo<sc_int<24>> i_R;
	sc_fifo<sc_int<16>> i_T;

    sc_fifo_out<sc_int<48>> o_RMX;
  	sc_fifo_out<sc_int<48>> o_RMY;
    sc_fifo_out<sc_int<48>> o_RMZ;
	sc_fifo_out<sc_int<5>> o_TX;
	sc_fifo_out<sc_int<5>> o_TY;
	sc_fifo_out<sc_int<5>> o_TZ;

    SC_HAS_PROCESS(CORDIC);
    CORDIC(sc_module_name n);
    ~CORDIC(); 
private:  
    sc_int<16> table[10] = {6800, 3593, 1824, 915, 457, 229, 114, 57, 28, 14};
    sc_int<16> theta;
    sc_int<16> x, y, reg_x;
    sc_uint<8> rx, ry, rz;
	sc_int<5> tx, ty, tz;
    sc_int<16> RM[9];
    sc_int<24> tmp;
    void do_CORDIC();
};
#endif