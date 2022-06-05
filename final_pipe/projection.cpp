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
	i_rotate.clk_rst(i_clk, i_rst);
	o_result.clk_rst(i_clk, i_rst);
#endif
}

Projection::~Projection() {};

void Projection::do_Projection() {
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("projection_reset");
		i_rotate.reset();
		o_result.reset();
	#endif
		wait();

	while(true) {
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("input");
			tmp = i_rotate.get();
		#else
			tmp = i_rotate.read();
		#endif
		wait();
		
		p_x = tmp.range(15, 0);
		p_y = tmp.range(31, 16);
		p_z = tmp.range(47, 32);

		reg_z = p_z >> 10;

		// projection
		{
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
