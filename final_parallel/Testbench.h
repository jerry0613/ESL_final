#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
#include <systemc>
#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#endif

using namespace std;
using namespace sc_core;
using namespace sc_dt;

class Testbench : public sc_module {
public:
	sc_in_clk i_clk;
	sc_out < bool > o_rst_sin, o_rst_shader, o_rst_projection;
#ifndef NATIVE_SYSTEMC
	cynw_p2p< sc_int<48> >::base_in i_result;

	cynw_p2p< sc_int<32> >::base_out o_in;
	cynw_p2p< sc_int<5> >::base_out o_TX;
	cynw_p2p< sc_int<5> >::base_out o_TY;
	cynw_p2p< sc_int<5> >::base_out o_TZ;
	cynw_p2p< sc_int<48> >::base_out o_xyz1;
	cynw_p2p< sc_int<48> >::base_out o_xyz2;
	cynw_p2p< sc_int<48> >::base_out o_xyz3;
#else
	sc_fifo_in< sc_int<48> > i_result;

	sc_fifo_out< sc_int<32> > o_in;
	sc_fifo_out< sc_int<5> > o_TX;
	sc_fifo_out< sc_int<5> > o_TY;
	sc_fifo_out< sc_int<5> > o_TZ;
	sc_fifo_out< sc_int<48> > o_xyz1;
	sc_fifo_out< sc_int<48> > o_xyz2;
	sc_fifo_out< sc_int<48> > o_xyz3;
#endif

	SC_HAS_PROCESS(Testbench);
	Testbench(sc_module_name n);
	~Testbench();
private:
	sc_time total_start_time;
	sc_time total_run_time;

	void feed();
	void fetch();
};
#endif
