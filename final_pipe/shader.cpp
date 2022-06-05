#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
#include <iomanip>
#include "shader.h"

sc_int<16> RM[9];
sc_int<16> p[3];
sc_int<48> tmp[3];
sc_uint<5> shift[3] = {14, 14, 16};

Shader::Shader(sc_module_name n): sc_module(n) {
  	SC_THREAD(do_Shader);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_TX.clk_rst(i_clk, i_rst);
	i_TY.clk_rst(i_clk, i_rst);
	i_TZ.clk_rst(i_clk, i_rst);
	i_xyz.clk_rst(i_clk, i_rst);
	i_RMX.clk_rst(i_clk, i_rst);
	i_RMY.clk_rst(i_clk, i_rst);
	i_RMZ.clk_rst(i_clk, i_rst);
	o_rotate.clk_rst(i_clk, i_rst);
	HLS_FLATTEN_ARRAY( RM );
	HLS_FLATTEN_ARRAY( p );
	HLS_FLATTEN_ARRAY( tmp );
	HLS_FLATTEN_ARRAY( shift );
#endif
}

Shader::~Shader() {}

void Shader::do_Shader() {
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("shader_reset");
		i_TX.reset();
		i_TY.reset();
		i_TZ.reset();
		i_xyz.reset();
		i_RMX.reset();
		i_RMY.reset();
		i_RMZ.reset();
		o_rotate.reset();
	#endif
		wait();

	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("input");
		tx = i_TX.get();
		ty = i_TY.get();
		tz = i_TZ.get();
		tmp[0] = i_RMX.get();
		tmp[1] = i_RMY.get();
		tmp[2] = i_RMZ.get();
	#else
		tx = i_TX.read();
		ty = i_TY.read();
		tz = i_TZ.read();
		tmp[0] = i_RMX.read();
		tmp[1] = i_RMY.read();
		tmp[2] = i_RMZ.read();
	#endif
	wait();

	RM[0] = tmp[0].range(15, 0);
	RM[1] = tmp[0].range(31, 16);
	RM[2] = tmp[0].range(47, 32);
	RM[3] = tmp[1].range(15, 0);
	RM[4] = tmp[1].range(31, 16);
	RM[5] = tmp[1].range(47, 32);
	RM[6] = tmp[2].range(15, 0);
	RM[7] = tmp[2].range(31, 16);
	RM[8] = tmp[2].range(47, 32);

	tmp[0] = tx << 11;
	tmp[1] = ty << 11;
	tmp[2] = (tz << 9) - (3 << 12);

	while(true) {
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("input");
			out = i_xyz.get();
		#else
			out = i_xyz.read();
		#endif

		x = out.range(15, 0);
		y = out.range(31, 16);
		z = out.range(47, 32);
		wait();

		// rotation
		for (int i = 0; i < 3; i++) {
			HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
			p[i] = ((x*RM[3*i] + y*RM[3*i+1] + (z*RM[3*i+2] << 2)) >> shift[i]) + tmp[i];
			wait();
		}
		
		// output
		out.range(15, 0) = p[0];
		out.range(31, 16) = p[1];
		out.range(47, 32) = p[2];
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
			o_rotate.put(out);
		#else
			o_rotate.write(out);
		#endif
	}
}
