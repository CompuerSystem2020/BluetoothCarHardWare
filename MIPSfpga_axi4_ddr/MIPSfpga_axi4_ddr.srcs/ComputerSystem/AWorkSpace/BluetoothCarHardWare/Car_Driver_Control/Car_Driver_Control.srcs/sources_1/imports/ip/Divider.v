`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/09/29 15:55:18
// Design Name: 
// Module Name: Divider
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


module Divider #
(
    parameter integer DIR_CLK_PERIOD = 2
)
(
    input clk,
    output dir_c 
);
    reg [31:0] count;
    reg flag = 0;
    always @(posedge clk)
    begin
        if (count < DIR_CLK_PERIOD)
            count <= count + 1;
        else 
            begin
                count <= 0;
                flag = ~flag;
            end
    end
        
    assign dir_c = flag;            
endmodule
