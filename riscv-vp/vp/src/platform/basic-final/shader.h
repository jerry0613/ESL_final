#ifndef SHADER_H_
#define SHADER_H_

using namespace std;
#include <systemc>
using namespace sc_core;
using namespace sc_dt;

// define some parameters
#define w_r 4 // fl = 1
#define w_t 3 // fl = 1
#define w_n 49152 // fl = 14
#define w_f 163840 // fl = 14
#define w_z1 4179 // fl = 12
#define w_z2 3310 // fl = 14

class Shader: public sc_module {
public:
    sc_fifo<sc_int<16>> i_T;
    sc_fifo<sc_int<16>> i_x;
    sc_fifo<sc_int<16>> i_y;
    sc_fifo<sc_int<16>> i_z;

    sc_fifo_in<sc_int<48>> i_RMX;
  	sc_fifo_in<sc_int<48>> i_RMY;
    sc_fifo_in<sc_int<48>> i_RMZ;

    sc_fifo<sc_int<16>> o_x;
    sc_fifo<sc_int<16>> o_y;
    sc_fifo<sc_int<16>> o_z;

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