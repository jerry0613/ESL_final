#include <iomanip>
#include "shader.h"

Shader::Shader(sc_module_name n): sc_module(n){
  	SC_THREAD(do_Shader_X);
	SC_THREAD(do_Shader_Y);
	SC_THREAD(do_Shader_Z);
}

Shader::~Shader() = default;

void Shader::do_Shader_X() {
	sc_int<16> x, y, z;
	// read translation
	tx = i_TX.read();
	// read rotation matrix
	tmp_x = i_RMX.read();
	RM[0] = tmp_x.range(15, 0);
	RM[1] = tmp_x.range(31, 16);
	RM[2] = tmp_x.range(47, 32);

	while(true) {
		// read input
		x = i_x1.read();
		y = i_y1.read();
		z = i_z1.read();
		// rotation
		p_x = (x*RM[0] + y*RM[1] + (z*RM[2] << 2)) >> 14;
		// translation
		p_x += (tx << 11);	
		// output
		o_rx.write(p_x);
	}
}

void Shader::do_Shader_Y() {
	sc_int<16> x, y, z;
	// read translation
	ty = i_TY.read();
	// read rotation matrix
	tmp_y = i_RMY.read();
	RM[3] = tmp_y.range(15, 0);
	RM[4] = tmp_y.range(31, 16);
	RM[5] = tmp_y.range(47, 32);

	while(true) {
		// read input
		x = i_x2.read();
		y = i_y2.read();
		z = i_z2.read();
		// rotation
		p_y = (x*RM[3] + y*RM[4] + (z*RM[5] << 2)) >> 14;	
		// translation
		p_y += (ty << 11);	
		// output
		o_ry.write(p_y);
	}
}

void Shader::do_Shader_Z() {
	sc_int<16> x, y, z;
	// read translation
	tz = i_TZ.read();
	// read rotation matrix
	tmp_z = i_RMZ.read();
	RM[6] = tmp_z.range(15, 0);
	RM[7] = tmp_z.range(31, 16);
	RM[8] = tmp_z.range(47, 32);

	while(true) {
		// read input
		x = i_x3.read();
		y = i_y3.read();
		z = i_z3.read();
		// rotation
		p_z = (x*RM[6] + y*RM[7] + (z*RM[8] << 2)) >> 16;
		// translation
		p_z += (tz << 9) - (3 << 12);	
		// output
		o_rz.write(p_z);
	}
}

