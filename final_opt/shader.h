#ifndef SHADER_H_
#define SHADER_H_

// define some parameters
#define w_r 4 // fl = 1
#define w_t 3 // fl = 1
#define w_n 49152 // fl = 14
#define w_z1 4179 // fl = 12
#define w_z2 3310 // fl = 14

#include <systemc>
#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

using namespace std;
using namespace sc_core;
using namespace sc_dt;

class Shader: public sc_module {
public:
    sc_in_clk i_clk;
	sc_in < bool > i_rst;

#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_int<5> >::in i_TX;
	cynw_p2p< sc_int<5> >::in i_TY;
	cynw_p2p< sc_int<5> >::in i_TZ;
    cynw_p2p< sc_int<48> >::in i_xyz;
    cynw_p2p< sc_int<48> >::in i_RMX;
  	cynw_p2p< sc_int<48> >::in i_RMY;
    cynw_p2p< sc_int<48> >::in i_RMZ;

    cynw_p2p< sc_int<48> >::out o_result;
#else
    sc_fifo_in< sc_int<5> > i_TX;
	sc_fifo_in< sc_int<5> > i_TY;
	sc_fifo_in< sc_int<5> > i_TZ;
    sc_fifo_in< sc_int<48> > i_xyz;
    sc_fifo_in< sc_int<48> > i_RMX;
  	sc_fifo_in< sc_int<48> > i_RMY;
    sc_fifo_in< sc_int<48> > i_RMZ;

    sc_fifo_out<sc_int<48>> o_result;
#endif

    SC_HAS_PROCESS(Shader);
    Shader(sc_module_name n);
    ~Shader(); 
private:
    sc_int<16> x, y, z;
    sc_int<5> tx, ty, tz;
    sc_int<6> reg_z;
    sc_int<48> out;

    void do_Shader();
};
#endif
