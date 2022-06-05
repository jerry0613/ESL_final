#ifndef PROJECTION_H_
#define PROJECTION_H_

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

class Projection: public sc_module {
public:
    sc_fifo_in<sc_int<16>> i_rx;
    sc_fifo_in<sc_int<16>> i_ry;
    sc_fifo_in<sc_int<16>> i_rz;

    sc_fifo<sc_int<16>> o_x;
    sc_fifo<sc_int<16>> o_y;
    sc_fifo<sc_int<16>> o_z;

    SC_HAS_PROCESS(Projection);
    Projection(sc_module_name n);
    ~Projection(); 
private:
    sc_int<16> x, y, z;
    sc_int<16> p_x, p_y, p_z;

    void do_Projection();
};
#endif