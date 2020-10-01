`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/09/29 15:54:38
// Design Name: 
// Module Name: Pwm
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 根据speed计算duty_cycle的值，之后根据这个值输出不同的占空比信号
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Pwm
(
	input clk,
	input reset,
	input [3:0] speed,
	output reg pwm_out
);
	reg [20:0] count = 0;
	reg [31:0] duty_cycle = 0;
	always @(speed)
		duty_cycle <= speed << 17;
	always @(posedge clk)
    begin
        if (reset == 1'b0)
        begin
            count <= 0;
            duty_cycle <= 0;
        end
        else 
        begin
            count <= count + 1;
            if (count < duty_cycle)
                pwm_out <= 1;
            else
                pwm_out <= 0;
        end
    end
endmodule
