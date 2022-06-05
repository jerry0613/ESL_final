#include <iomanip>
#include "shader.h"

Shader::Shader(sc_module_name n): sc_module(n){
  	SC_THREAD(do_Shader);
}

Shader::~Shader() = default;

void Shader::do_Shader() {
	// read translation
	tmp = i_T.read();
	tx = tmp.range(4, 0);
	ty = tmp.range(9, 5);
	tz = tmp.range(14, 10);
	// read rotation matrix
	tmp = i_RMX.read();
	RM[0] = tmp.range(15, 0);
	RM[1] = tmp.range(31, 16);
	RM[2] = tmp.range(47, 32);

	tmp = i_RMY.read();
	RM[3] = tmp.range(15, 0);
	RM[4] = tmp.range(31, 16);
	RM[5] = tmp.range(47, 32);

	tmp = i_RMZ.read();
	RM[6] = tmp.range(15, 0);
	RM[7] = tmp.range(31, 16);
	RM[8] = tmp.range(47, 32);

	while(true) {
		// read input
		x = i_x.read();
		y = i_y.read();
		z = i_z.read();

		// rotation
		p_x = (x*RM[0] + y*RM[1] + (z*RM[2] << 2)) >> 14;
		p_y = (x*RM[3] + y*RM[4] + (z*RM[5] << 2)) >> 14;
		p_z = (x*RM[6] + y*RM[7] + (z*RM[8] << 2)) >> 16;
		
		// translation
		p_x += (tx << 11);
		p_y += (ty << 11);
		p_z += (tz << 9) - (3 << 12);
		
		// projection
		p_x = (w_n / w_r * p_x >> 11) / (-p_z >> 10);
		p_y = (w_n / w_t * p_y >> 11) / (-p_z >> 10);
		p_z = w_z1 + (w_z2 / (p_z >> 10));
		
		// output
		o_x.write(p_x);
		o_y.write(p_y);
		o_z.write(p_z);
	}
}
