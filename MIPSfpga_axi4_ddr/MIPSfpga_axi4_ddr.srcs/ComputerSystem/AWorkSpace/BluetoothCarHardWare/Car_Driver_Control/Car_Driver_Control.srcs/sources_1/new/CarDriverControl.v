`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/09/30 18:43:14
// Design Name: 
// Module Name: CarDriverControl
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module CarDriverControl(
    input clk,
    input[11:0] param,
    output dir_clk,
    output dir_enable,
    output dir_serial,
    output dir_latch,
    output[3:0] wheel
    );
    Divider myDivider(clk, dir_clk);
    Direction myDirection(dir_clk, param[11:4], dir_serial, dir_latch);
    Pwm p0(clk, param[3:0], wheel[0]);
    Pwm p1(clk, param[3:0], wheel[1]);
    Pwm p2(clk, param[3:0], wheel[2]);
    Pwm p3(clk, param[3:0], wheel[3]);
    assign dir_enable = 0;
endmodule
