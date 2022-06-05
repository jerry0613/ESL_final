#include <cstdio>
#include <cstdlib>
#include "Testbench.h"
#include <cmath>
#include <fstream>

Testbench::Testbench(sc_module_name n) : sc_module(n) {
	SC_THREAD(feed);
	sensitive << i_clk.pos();
	dont_initialize();

	SC_THREAD(fetch);
	sensitive << i_clk.pos();
	dont_initialize();
}

Testbench::~Testbench() {
	cout << "Total run time = " << total_run_time << endl;
}

void Testbench::feed() {
	int theta = 45;
	float R[3] = {1, 1, 0};
	float T[3] = {0, 0, 0};
	float V[36][3] = {
		{-0.5f, -0.5f, -0.5f},  
         {0.5f, -0.5f, -0.5f},  
         {0.5f,  0.5f, -0.5f},  
         {0.5f,  0.5f, -0.5f}, 
        {-0.5f,  0.5f, -0.5f},  
        {-0.5f, -0.5f, -0.5f},  

        {-0.5f, -0.5f,  0.5f},  
         {0.5f, -0.5f,  0.5f},  
         {0.5f,  0.5f,  0.5f}, 
         {0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f},  
        {-0.5f, -0.5f,  0.5f},  

        {-0.5f,  0.5f,  0.5f},  
        {-0.5f,  0.5f, -0.5f},  
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f,  0.5f},  
        {-0.5f,  0.5f,  0.5f}, 

         {0.5f,  0.5f,  0.5f},  
         {0.5f,  0.5f, -0.5f},  
         {0.5f, -0.5f, -0.5f},  
         {0.5f, -0.5f, -0.5f}, 
         {0.5f, -0.5f,  0.5f},  
         {0.5f,  0.5f,  0.5f},  

        {-0.5f, -0.5f, -0.5f}, 
         {0.5f, -0.5f, -0.5f}, 
         {0.5f, -0.5f,  0.5f}, 
         {0.5f, -0.5f,  0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 

        {-0.5f,  0.5f, -0.5f},  
         {0.5f,  0.5f, -0.5f},  
         {0.5f,  0.5f,  0.5f},  
         {0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f},  
        {-0.5f,  0.5f, -0.5f} 
	};
	sc_int<48> tmp;	
	float len;

#ifndef NATIVE_SYSTEMC
 	o_in.reset();
	o_xyz1.reset();
	o_xyz2.reset();
	o_xyz3.reset();
	o_TX.reset();
	o_TY.reset();
	o_TZ.reset();
#endif
  	o_rst_sin.write(false);
	o_rst_shader.write(false);
	o_rst_projection.write(false);
	wait(5);
	o_rst_sin.write(true);
	o_rst_shader.write(true);
	o_rst_projection.write(true);
	wait(1);
	
	len = sqrt(R[0]*R[0] + R[1]*R[1] + R[2]*R[2]);
	tmp.range(7, 0) = theta;
	tmp.range(15, 8) = (int)(R[0] * (1 << 7) / len);
	tmp.range(23, 16) = (int)(R[1] * (1 << 7) / len);
	tmp.range(31, 24) = (int)(R[2] * (1 << 7) / len);
	#ifndef NATIVE_SYSTEMC
		o_in.put(tmp);
    #else
    	o_in.write(tmp);
    #endif
	// translation
	tmp.range(4, 0) = (int)(T[0]*8);
	tmp.range(9, 5) = (int)(T[1]*8);
	tmp.range(14, 10) = (int)(T[2]*8);
	#ifndef NATIVE_SYSTEMC
		o_TX.put(tmp.range(4, 0));
		o_TY.put(tmp.range(9, 5));
		o_TZ.put(tmp.range(14, 10));
    #else
    	o_TX.write(tmp.range(4, 0));
		o_TY.write(tmp.range(9, 5));
		o_TZ.write(tmp.range(14, 10));
    #endif

	// input and output x, y, z
	for (int i = 0; i < 36; i++) {
		tmp.range(15, 0) = (int)(V[i][0] * 16384);
		tmp.range(31, 16) = (int)(V[i][1] * 16384);
		tmp.range(47, 32) = (int)(V[i][2] * (1 << 12));
		#ifndef NATIVE_SYSTEMC
			o_xyz1.put(tmp);
			o_xyz2.put(tmp);
			o_xyz3.put(tmp);
		#else
			o_xyz1.write(tmp);
			o_xyz2.write(tmp);
			o_xyz3.write(tmp);
		#endif
	}
}

void Testbench::fetch() {
	ofstream file;
	float x, y, z;
	sc_dt::sc_int<48> tmp;
	sc_dt::sc_int<16> tmp_x, tmp_y, tmp_z;

	file.open("vertex.txt");
#ifndef NATIVE_SYSTEMC
  	i_result.reset();
#endif
	wait(5);
	wait(1);
	total_start_time = sc_time_stamp();

	for (int i = 0; i < 36; i++) {
		#ifndef NATIVE_SYSTEMC
			tmp = i_result.get();
		#else
			tmp = i_result.read();
		#endif
		tmp_x = tmp.range(15, 0);
		tmp_y = tmp.range(31, 16);
		tmp_z = tmp.range(47, 32);
		x = (float)(tmp_x) / 16384;
		y = (float)(tmp_y) / 16384;
		z = (float)(tmp_z) / (1 << 12);
		file << "[" << x << ", " << y << ", " << z << "]," << endl;
	}

	total_run_time = sc_time_stamp() - total_start_time;
	file.close();
	sc_stop();
}
