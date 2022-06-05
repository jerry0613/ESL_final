#ifndef PROJECTION_H_
#define PROJECTION_H_

#include <systemc>
#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

using namespace std;
using namespace sc_core;
using namespace sc_dt;

// define some parameters
#define w_r 4 // fl = 1
#define w_t 3 // fl = 1
#define w_n 49152 // fl = 14
#define w_z1 4179 // fl = 12
#define w_z2 3310 // fl = 14

class Projection: public sc_module {
public:
    sc_in_clk i_clk;
	sc_in < bool > i_rst;
    
#ifndef NATIVE_SYSTEMC
    cynw_p2p< sc_int<48> >::in i_rotate;

    cynw_p2p< sc_int<48> >::out o_result;
#else
    sc_fifo_in<sc_int<48>> i_rotate;

    sc_fifo_out<sc_int<48>> o_result;
#endif

    SC_HAS_PROCESS(Projection);
    Projection(sc_module_name n);
    ~Projection(); 
private:
    sc_int<16> p_x, p_y, p_z;
    sc_int<6> reg_z;
    sc_int<48> tmp;

    void do_Projection();
};
#endif