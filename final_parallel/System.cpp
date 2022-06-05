#include "System.h"
System::System( sc_module_name n): sc_module( n ), tb("tb"), CORDIC("CORDIC"), shader("Shader"), projection("Projection"),
		clk("clk", CLOCK_PERIOD, SC_NS), rst_sin("rst_sin")
{
	tb.i_clk(clk);
	tb.o_rst_sin(rst_sin);
	tb.o_rst_shader(rst_shader);
	tb.o_rst_projection(rst_projection);
	tb.i_result(c_result);
	tb.o_in(c_in);
	tb.o_xyz1(c_xyz1);
	tb.o_xyz2(c_xyz2);
	tb.o_xyz3(c_xyz3);
	tb.o_TX(c_TX);
	tb.o_TY(c_TY);
	tb.o_TZ(c_TZ);
	
	CORDIC.i_clk(clk);
	CORDIC.i_rst(rst_sin);
	CORDIC.i_in(c_in);
	CORDIC.o_RMX(c_RMX);
	CORDIC.o_RMY(c_RMY);
	CORDIC.o_RMZ(c_RMZ);

	shader.i_clk(clk);
	shader.i_rst(rst_shader);
	shader.i_TX(c_TX);
	shader.i_TY(c_TY);
	shader.i_TZ(c_TZ);
	shader.i_xyz1(c_xyz1);
	shader.i_xyz2(c_xyz2);
	shader.i_xyz3(c_xyz3);
	shader.i_RMX(c_RMX);
	shader.i_RMY(c_RMY);
	shader.i_RMZ(c_RMZ);
	shader.o_rox(c_rox);
	shader.o_roy(c_roy);
	shader.o_roz(c_roz);

	projection.i_clk(clk);
	projection.i_rst(rst_projection);
	projection.i_rox(c_rox);
	projection.i_roy(c_roy);
	projection.i_roz(c_roz);
	projection.o_result(c_result);
}

System::~System() {}
