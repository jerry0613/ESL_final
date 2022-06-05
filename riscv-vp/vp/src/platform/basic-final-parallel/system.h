#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "cordic.h"
#include "shader.h"
#include "projection.h"

using namespace sc_core;
using namespace sc_dt;

const int THETA_ADDR = 0x00000000;
const int R_ADDR = 0x00000004;
const int T_ADDR = 0x00000008;
const int IN_X_ADDR = 0x0000000c;
const int IN_Y_ADDR = 0x00000010;
const int IN_Z_ADDR = 0x00000014;
const int OUT_X_ADDR = 0x00000018;
const int OUT_Y_ADDR = 0x0000001c;
const int OUT_Z_ADDR = 0x00000020;

union package {
  unsigned char uc[4];
};

class System: public sc_module
{
public:
  	tlm_utils::simple_target_socket<System> tsock;

	SC_HAS_PROCESS(System);
	System(sc_module_name n);
	~System();
private:
	sc_fifo<sc_int<48>> c_RMX;
  	sc_fifo<sc_int<48>> c_RMY;
	sc_fifo<sc_int<48>> c_RMZ;
	sc_fifo<sc_int<5>> c_TX;
	sc_fifo<sc_int<5>> c_TY;
	sc_fifo<sc_int<5>> c_TZ;
	sc_fifo<sc_int<16>> c_rx;
	sc_fifo<sc_int<16>> c_ry;
	sc_fifo<sc_int<16>> c_rz;

	CORDIC cordic;
	Shader shader;
	Projection projection;

  	unsigned int base_offset = 0;
	void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay);
};
#endif