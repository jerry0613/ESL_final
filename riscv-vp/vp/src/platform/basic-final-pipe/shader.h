#ifndef SHADER_H_
#define SHADER_H_

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;

class Shader: public sc_module {
public:
    sc_fifo<sc_int<16>> i_T;
    sc_fifo<sc_int<16>> i_x;
    sc_fifo<sc_int<16>> i_y;
    sc_fifo<sc_int<16>> i_z;

    sc_fifo_in<sc_int<48>> i_RMX;
  	sc_fifo_in<sc_int<48>> i_RMY;
    sc_fifo_in<sc_int<48>> i_RMZ;

    sc_fifo_out<sc_int<16>> o_rx;
    sc_fifo_out<sc_int<16>> o_ry;
    sc_fifo_out<sc_int<16>> o_rz;

    SC_HAS_PROCESS(Shader);
    Shader(sc_module_name n);
    ~Shader(); 
private:
    sc_int<16> x, y, z;
    sc_int<16> p_x, p_y, p_z;
    sc_int<5> tx, ty, tz;
    sc_int<48> tmp;
    sc_int<16> RM[9];
    void do_Shader();
};
#endif