#ifndef SHADER_H_
#define SHADER_H_

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
    cynw_p2p< sc_int<48> >::in i_xyz1;
    cynw_p2p< sc_int<48> >::in i_xyz2;
    cynw_p2p< sc_int<48> >::in i_xyz3;
    cynw_p2p< sc_int<48> >::in i_RMX;
  	cynw_p2p< sc_int<48> >::in i_RMY;
    cynw_p2p< sc_int<48> >::in i_RMZ;

    cynw_p2p< sc_int<16> >::out o_rox;
    cynw_p2p< sc_int<16> >::out o_roy;
    cynw_p2p< sc_int<16> >::out o_roz;
#else
    sc_fifo_in< sc_int<5> > i_TX;
	sc_fifo_in< sc_int<5> > i_TY;
	sc_fifo_in< sc_int<5> > i_TZ;
    sc_fifo_in< sc_int<48> > i_xyz1;
    sc_fifo_in< sc_int<48> > i_xyz2;
    sc_fifo_in< sc_int<48> > i_xyz3;
    sc_fifo_in< sc_int<48> > i_RMX;
  	sc_fifo_in< sc_int<48> > i_RMY;
    sc_fifo_in< sc_int<48> > i_RMZ;

    sc_fifo_out<sc_int<16>> o_rox;
    sc_fifo_out<sc_int<16>> o_roy;
    sc_fifo_out<sc_int<16>> o_roz;
#endif

    SC_HAS_PROCESS(Shader);
    Shader(sc_module_name n);
    ~Shader(); 
private:
    sc_int<16> p_x, p_y, p_z;
    sc_int<5> tx, ty, tz;
    sc_int<48> out;

    void do_Shader_X();
    void do_Shader_Y();
    void do_Shader_Z();
};
#endif
