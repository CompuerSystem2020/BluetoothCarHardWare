`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/09/29 15:55:01
// Design Name: 
// Module Name: Direction
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


module Direction
(
	input clk,
	input [7:0] direction,
	output reg dir_serial,
	output reg dir_latch
);
	reg [3:0] state = 0;
	always @(negedge clk)
    begin
        case (state)
            0 : begin
                dir_serial <= direction[0];
                dir_latch <= 0;
                state <= 1;
            end
            1 : begin
                dir_serial <= direction[1];
                dir_latch <= 0;
                state <= 2;
            end
            2 : begin
                dir_serial <= direction[2];
                dir_latch <= 0;
                state <= 3;
            end
            3 : begin
                dir_serial <= direction[3];
                dir_latch <= 0;
                state <= 4;
            end
            4 : begin
                dir_serial <= direction[4];
                dir_latch <= 0;
                state <= 5;
            end
            5 : begin
                dir_serial <= direction[5];
                dir_latch <= 0;
                state <= 6;
            end
            6 : begin
                dir_serial <= direction[6];
                dir_latch <= 0;
                state <= 7;
            end
            7 : begin
                dir_serial <= direction[7];
                dir_latch <= 0;
                state <= 8;
            end
            8 : begin
                dir_latch <= 1;
                state <= 0;
            end
            default : begin
                state <= 0;
            end
        endcase
    end
endmodule
