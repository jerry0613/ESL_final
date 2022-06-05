#include <iomanip>
#include "cordic.h"

CORDIC::CORDIC(sc_module_name n): sc_module(n) {
 	 SC_THREAD(do_CORDIC);
}

CORDIC::~CORDIC() = default;

void CORDIC::do_CORDIC() {
	while(true) {
		theta = (i_theta.read() - 45) << 8;

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

		// read rotation
		tmp = i_R.read();
		rx = tmp.range(7, 0);
		ry = tmp.range(15, 8);
		rz = tmp.range(23, 16);
		// read translation
		tmp = i_T.read();
		tx = tmp.range(4, 0);
		ty = tmp.range(9, 5);
		tz = tmp.range(14, 10);
		// compute rotation matrix
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

		o_RMX.write((RM[2] << 32) + (RM[1] << 16) + RM[0]);
		o_RMY.write((RM[5] << 32) + (RM[4] << 16) + RM[3]);
		o_RMZ.write((RM[8] << 32) + (RM[7] << 16) + RM[6]);
		o_TX.write(tx);
		o_TY.write(ty);
		o_TZ.write(tz);
	}
}
