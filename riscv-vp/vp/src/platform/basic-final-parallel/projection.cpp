#include <iomanip>
#include "projection.h"

Projection::Projection(sc_module_name n): sc_module(n){
  	SC_THREAD(do_Projection);
}

Projection::~Projection() = default;

void Projection::do_Projection() {
	while(true) {
		// read input
		x = i_rx.read();
		y = i_ry.read();
		z = i_rz.read();
		
		// projection
		p_x = (w_n / w_r * x >> 11) / (-z >> 10);
		p_y = (w_n / w_t * y >> 11) / (-z >> 10);
		p_z = w_z1 + (w_z2 / (z >> 10));
		
		// output
		o_x.write(p_x);
		o_y.write(p_y);
		o_z.write(p_z);
	}
}
