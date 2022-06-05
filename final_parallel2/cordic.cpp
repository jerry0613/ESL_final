// 1234
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif
#include <iomanip>
#include "cordic.h"

const sc_uint<13> table[10] = {6800, 3593, 1824, 915, 457, 229, 114, 57, 28, 14};

CORDIC::CORDIC(sc_module_name n): sc_module(n) {
	SC_THREAD(do_CORDIC);
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
	i_in.clk_rst(i_clk, i_rst);
	o_RMX.clk_rst(i_clk, i_rst);
	o_RMY.clk_rst(i_clk, i_rst);
	o_RMZ.clk_rst(i_clk, i_rst);
	HLS_FLATTEN_ARRAY( table );
	HLS_FLATTEN_ARRAY( RM );
#endif
}

CORDIC::~CORDIC() {}

void CORDIC::do_CORDIC() {
	#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("cordic_reset");
		i_in.reset();
		o_RMX.reset();
		o_RMY.reset();
		o_RMZ.reset();
	#endif
		wait();

	while(true) {
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("input");
			tmp = i_in.get();
			wait();
		#else
			tmp = i_in.read();
		#endif
		theta = (tmp.range(7, 0) - 45) << 8;
		rx = tmp.range(15, 8);
		ry = tmp.range(23, 16);
		rz = tmp.range(31, 24);
		{
			y = X0;
			x = X0;
			for (int i = 0; i < 10; i++) {
				if (theta < 0) {
				theta = theta + table[i];
				reg_x = x;
				x = x + (y >> (i+1));
				y = y - (reg_x >> (i+1)); 
				}
				else {
				theta = theta - table[i];
				reg_x = x;
				x = x - (y >> (i+1));
				y = y + (reg_x >> (i+1)); 
				}
			}
		}
		{
			tmp = (1 << 14) - x; // 1 - cos
			RM[0] = x + (rx*rx*tmp >> 14);
			RM[1] = (rx*ry*tmp >> 14) - (rz*y >> 7);
			RM[2] = (rx*rz*tmp >> 14) + (ry*y >> 7);
			RM[3] = (ry*rx*tmp >> 14) + (rz*y >> 7);
			RM[4] = x + (ry*ry*tmp >> 14);
			RM[5] = (ry*rz*tmp >> 14) - (rx*y >> 7);
			RM[6] = (rz*rx*tmp >> 14) - (ry*y >> 7);
			RM[7] = (rz*ry*tmp >> 14) + (rx*y >> 7);
			RM[8] = x + (rz*rz*tmp >> 14);
		}
		#ifndef NATIVE_SYSTEMC
			HLS_DEFINE_PROTOCOL("output");
			o_RMX.put(RM[0] + (RM[1] << 16) + (RM[2] << 32));
			o_RMY.put(RM[3] + (RM[4] << 16) + (RM[5] << 32));
			o_RMZ.put(RM[6] + (RM[7] << 16) + (RM[8] << 32));
		#else
			o_RMX.write(RM[0] + (RM[1] << 16) + (RM[2] << 32));
			o_RMY.write(RM[3] + (RM[4] << 16) + (RM[5] << 32));
			o_RMZ.write(RM[6] + (RM[7] << 16) + (RM[8] << 32));
		#endif
	}
}
