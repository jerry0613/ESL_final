#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
#include <iomanip>
#include "projection.h"

Projection::Projection(sc_module_name n): sc_module(n){
  	SC_THREAD(do_Projection);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_rox.clk_rst(i_clk, i_rst);
	i_roy.clk_rst(i_clk, i_rst);
	i_roz.clk_rst(i_clk, i_rst);
	o_result.clk_rst(i_clk, i_rst);
#endif
}

Projection::~Projection() {};

void Projection::do_Projection() {
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("projection_reset");
		i_rox.reset();
		i_roy.reset();
		i_roz.reset();
		o_result.reset();
	#endif
		wait();

	while(true) {
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("input");
			p_x = i_rox.get();
			p_y = i_roy.get();
			p_z = i_roz.get();
		#else
			p_x = i_rox.read();
			p_y = i_roy.read();
			p_z = i_roz.read();
		#endif
		wait();

		reg_z = p_z >> 10;

		// projection
		{
			HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
			p_x = (w_n / w_r * p_x >> 11) / (-reg_z);
			p_y = (w_n / w_t * p_y >> 11) / (-reg_z);
			p_z = w_z1 + (w_z2 / reg_z);
			wait();
		}
		
		// output
		tmp.range(15, 0) = p_x;
		tmp.range(31, 16) = p_y;
		tmp.range(47, 32) = p_z;
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
			o_result.put(tmp);
		#else
			o_result.write(tmp);
		#endif
	}
}
