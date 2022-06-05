#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <systemc>
#include "Testbench.h"
#ifndef NATIVE_SYSTEMC
#include "cordic_wrap.h"
#include "shader_wrap.h"
#include "projection_wrap.h"
#else
#include "cordic.h"
#include "shader.h"
#include "projection.h"
#endif

using namespace sc_core;
using namespace sc_dt;

class System: public sc_module
{
public:
	SC_HAS_PROCESS( System );
	System( sc_module_name n );
	~System();
private:
  	Testbench tb;
#ifndef NATIVE_SYSTEMC
	CORDIC_wrapper CORDIC;
	Shader_wrapper shader;
	Projection_wrapper projection;
#else
	CORDIC CORDIC;
	Shader shader;
	Projection projection;
#endif

	sc_clock clk;
	sc_signal<bool> rst_sin, rst_shader, rst_projection;

#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_int<48> > c_result;

	cynw_p2p< sc_int<32> > c_in;
	cynw_p2p< sc_int<5> > c_TX;
	cynw_p2p< sc_int<5> > c_TY;
	cynw_p2p< sc_int<5> > c_TZ;
	cynw_p2p< sc_int<48> > c_RMX;
  	cynw_p2p< sc_int<48> > c_RMY;
    cynw_p2p< sc_int<48> > c_RMZ;
	cynw_p2p< sc_int<48> > c_xyz1;
	cynw_p2p< sc_int<48> > c_xyz2;
	cynw_p2p< sc_int<48> > c_xyz3;
	cynw_p2p< sc_int<16> > c_rox;
	cynw_p2p< sc_int<16> > c_roy;
	cynw_p2p< sc_int<16> > c_roz;
#else
	sc_fifo< sc_int<48> > c_result;

	sc_fifo< sc_int<32> > c_in;
	sc_fifo< sc_int<5> > c_TX;
	sc_fifo< sc_int<5> > c_TY;
	sc_fifo< sc_int<5> > c_TZ;
	sc_fifo< sc_int<48> > c_RMX;
  	sc_fifo< sc_int<48> > c_RMY;
    sc_fifo< sc_int<48> > c_RMZ;
	sc_fifo< sc_int<48> > c_xyz1;
	sc_fifo< sc_int<48> > c_xyz2;
	sc_fifo< sc_int<48> > c_xyz3;
	sc_fifo< sc_int<16> > c_rox;
	sc_fifo< sc_int<16> > c_roy;
	sc_fifo< sc_int<16> > c_roz;
#endif
};
#endif
