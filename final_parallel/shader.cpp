#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
#include <iomanip>
#include "shader.h"

sc_int<16> RM[9];
sc_int<16> p[3];
sc_int<48> tmp[3];
sc_int<48> in[3];

Shader::Shader(sc_module_name n): sc_module(n) {
  	SC_THREAD(do_Shader_X);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

	SC_THREAD(do_Shader_Y);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

	SC_THREAD(do_Shader_Z);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
#ifndef NATIVE_SYSTEMC
	i_TX.clk_rst(i_clk, i_rst);
	i_TY.clk_rst(i_clk, i_rst);
	i_TZ.clk_rst(i_clk, i_rst);
	i_xyz1.clk_rst(i_clk, i_rst);
	i_xyz2.clk_rst(i_clk, i_rst);
	i_xyz3.clk_rst(i_clk, i_rst);
	i_RMX.clk_rst(i_clk, i_rst);
	i_RMY.clk_rst(i_clk, i_rst);
	i_RMZ.clk_rst(i_clk, i_rst);
	o_rox.clk_rst(i_clk, i_rst);
	o_roy.clk_rst(i_clk, i_rst);
	o_roz.clk_rst(i_clk, i_rst);
	HLS_FLATTEN_ARRAY( RM );
	HLS_FLATTEN_ARRAY( p );
	HLS_FLATTEN_ARRAY( tmp );
	HLS_FLATTEN_ARRAY( in );
#endif
}

Shader::~Shader() {}

void Shader::do_Shader_X() {
	sc_int<16> x, y, z;
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("shader_reset");
		i_TX.reset();
		i_xyz1.reset();
		i_RMX.reset();
		o_rox.reset();
	#endif
		wait();

	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("input");
		tx = i_TX.get();
		tmp[0] = i_RMX.get();
	#else
		tx = i_TX.read();
		tmp[0] = i_RMX.read();
	#endif

	RM[0] = tmp[0].range(15, 0);
	RM[1] = tmp[0].range(31, 16);
	RM[2] = tmp[0].range(47, 32);

	tmp[0] = tx << 11;

	while(true) {
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("input");
			in[0] = i_xyz1.get();
		#else
			in[0] = i_xyz1.read();
		#endif

		x = in[0].range(15, 0);
		y = in[0].range(31, 16);
		z = in[0].range(47, 32);
		wait();

		{
			HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
			p_x = ((x*RM[0] + y*RM[1] + (z*RM[2] << 2)) >> 14) + tmp[0];
			wait();
		}

		// output
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
			o_rox.put(p_x);
		#else
			o_rox.write(p_x);
		#endif
	}
}

void Shader::do_Shader_Y() {
	sc_int<16> x, y, z;
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("shader_reset");
		i_TY.reset();
		i_xyz2.reset();
		i_RMY.reset();
		o_roy.reset();
	#endif
		wait();

	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("input");
		ty = i_TY.get();
		tmp[1] = i_RMY.get();
	#else
		ty = i_TY.read();
		tmp[1] = i_RMY.read();
	#endif

	RM[3] = tmp[1].range(15, 0);
	RM[4] = tmp[1].range(31, 16);
	RM[5] = tmp[1].range(47, 32);

	tmp[1] = ty << 11;

	while(true) {
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("input");
			in[1] = i_xyz2.get();
		#else
			in[1] = i_xyz2.read();
		#endif

		x = in[1].range(15, 0);
		y = in[1].range(31, 16);
		z = in[1].range(47, 32);
		wait();

		{
			HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
			p_y = ((x*RM[3] + y*RM[4] + (z*RM[5] << 2)) >> 14) + tmp[1];
			wait();
		}

		// output
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
			o_roy.put(p_y);
		#else
			o_roy.write(p_y);
		#endif
	}
}

void Shader::do_Shader_Z() {
	sc_int<16> x, y, z;
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("shader_reset");
		i_TZ.reset();
		i_xyz3.reset();
		i_RMZ.reset();
		o_roz.reset();
	#endif
		wait();

	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("input");
		tz = i_TZ.get();
		tmp[2] = i_RMZ.get();
	#else
		tz = i_TZ.read();
		tmp[2] = i_RMZ.read();
	#endif

	RM[6] = tmp[2].range(15, 0);
	RM[7] = tmp[2].range(31, 16);
	RM[8] = tmp[2].range(47, 32);

	tmp[2] = (tz << 9) - (3 << 12);

	while(true) {
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("input");
			in[2] = i_xyz3.get();
		#else
			in[2] = i_xyz3.read();
		#endif

		x = in[2].range(15, 0);
		y = in[2].range(31, 16);
		z = in[2].range(47, 32);
		wait();

		{
			HLS_CONSTRAIN_LATENCY(0, 1, "lat00");
			p_z = ((x*RM[6] + y*RM[7] + (z*RM[8] << 2)) >> 16) + tmp[2];
			wait();
		}

		// output
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
			o_roz.put(p_z);
		#else
			o_roz.write(p_z);
		#endif
	}
}