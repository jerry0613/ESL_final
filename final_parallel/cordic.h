#ifndef CORDIC_H_
#define CORDIC_H_
#define X0 9949

#include <systemc>
#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

using namespace std;
using namespace sc_core;
using namespace sc_dt;

class CORDIC: public sc_module {
public:
    sc_in_clk i_clk;
	sc_in < bool > i_rst;

#ifndef NATIVE_SYSTEMC
    cynw_p2p< sc_int<32> >::in i_in; // theta: 0~7, R: 8~31

    cynw_p2p< sc_int<48> >::out o_RMX;
    cynw_p2p< sc_int<48> >::out o_RMY;
    cynw_p2p< sc_int<48> >::out o_RMZ;
#else
    sc_fifo_in< sc_int<32> > i_in;

    sc_fifo_out< sc_int<48> > o_RMX;
    sc_fifo_out< sc_int<48> > o_RMY;
    sc_fifo_out< sc_int<48> > o_RMZ;
#endif

    SC_HAS_PROCESS(CORDIC);
    CORDIC(sc_module_name n);
    ~CORDIC(); 
private:
    sc_int<16> theta;
    sc_int<16> x, y, reg_x;
    sc_uint<8> rx, ry, rz;
    sc_int<16> RM[9];
    sc_int<32> tmp;

    void do_CORDIC();
};
#endif
