#include "System.h"

System::System(sc_module_name n): sc_module(n), tsock("t_skt"), base_offset(0),
									cordic("CORDIC"), shader("Shader"), projection("Projection")
{
	shader.i_RMX(c_RMX);
	shader.i_RMY(c_RMY);
	shader.i_RMZ(c_RMZ);
	cordic.o_RMX(c_RMX);
	cordic.o_RMY(c_RMY);
	cordic.o_RMZ(c_RMZ);
	shader.o_rx(c_rx);
	shader.o_ry(c_ry);
	shader.o_rz(c_rz);
	projection.i_rx(c_rx);
	projection.i_ry(c_ry);
	projection.i_rz(c_rz);

    tsock.register_b_transport(this, &System::blocking_transport);
}

System::~System() {}

void System::blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay) {
  	sc_dt::uint64 addr = payload.get_address();
  	addr -= base_offset;
  	unsigned char *data_ptr = payload.get_data_ptr();
  	package buffer;
  	sc_int<16> tmp;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
		case OUT_X_ADDR:
			tmp = projection.o_x.read();
			buffer.uc[0] = tmp.range(7, 0);
			buffer.uc[1] = tmp.range(15, 8);
			break; 
		case OUT_Y_ADDR:
			tmp = projection.o_y.read();
			buffer.uc[0] = tmp.range(7, 0);
			buffer.uc[1] = tmp.range(15, 8);
			break; 
		case OUT_Z_ADDR:
			tmp = projection.o_z.read();
			buffer.uc[0] = tmp.range(7, 0);
			buffer.uc[1] = tmp.range(15, 8);
			break; 
		default:
			buffer.uc[0] = 0;
			buffer.uc[1] = 0;
			buffer.uc[2] = 0;
			buffer.uc[3] = 0;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;
  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
		case THETA_ADDR:
			cordic.i_theta.write(data_ptr[0]);
			break;
		case R_ADDR:
			cordic.i_R.write((data_ptr[2] << 16) + (data_ptr[1] << 8) + data_ptr[0]);
			break;
		case T_ADDR:
			shader.i_T.write((data_ptr[1] << 8) + data_ptr[0]);
			break;
		case IN_X_ADDR:
			shader.i_x.write((data_ptr[1] << 8) + data_ptr[0]);
			break;
		case IN_Y_ADDR:
			shader.i_y.write((data_ptr[1] << 8) + data_ptr[0]);
			break;
		case IN_Z_ADDR:
			shader.i_z.write((data_ptr[1] << 8) + data_ptr[0]);
			break;
		default:
		break;
    }
    break;
  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}
