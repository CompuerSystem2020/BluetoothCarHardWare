module WheelController
(
	input clk,
	input [11:0] para,
	output dir_serial,
	output dir_enable,
	output dir_latch,
	output dir_clk,
	output [3:0] wheel
);
	wire clkk;
	divider div (clk, clkk);
	assign dir_clk = clkk;
	direction dd (clkk, para[11:4], dir_serial, dir_latch);
	pwm p1 (clk, para[3:0], wheel[3]);
	pwm p2 (clk, para[3:0], wheel[2]);
	pwm p3 (clk, para[3:0], wheel[1]);
	pwm p4 (clk, para[3:0], wheel[0]);
	assign dir_enable = 0;
endmodule