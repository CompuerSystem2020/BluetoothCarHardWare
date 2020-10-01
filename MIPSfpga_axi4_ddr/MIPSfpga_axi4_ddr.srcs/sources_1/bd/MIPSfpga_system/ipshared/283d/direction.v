module direction
(
	input clk,
	input [7:0] drt,
	output reg dir_serial,
	output reg dir_latch
);
	reg [3:0] stat = 0;
	always @(negedge clk)
		begin
			case (stat)
				0 : begin
					dir_serial <= drt[0];
					dir_latch <= 0;
					stat <= 1;
				end
				1 : begin
					dir_serial <= drt[1];
					dir_latch <= 0;
					stat <= 2;
				end
				2 : begin
					dir_serial <= drt[2];
					dir_latch <= 0;
					stat <= 3;
				end
				3 : begin
					dir_serial <= drt[3];
					dir_latch <= 0;
					stat <= 4;
				end
				4 : begin
					dir_serial <= drt[4];
					dir_latch <= 0;
					stat <= 5;
				end
				5 : begin
					dir_serial <= drt[5];
					dir_latch <= 0;
					stat <= 6;
				end
				6 : begin
					dir_serial <= drt[6];
					dir_latch <= 0;
					stat <= 7;
				end
				7 : begin
					dir_serial <= drt[7];
					dir_latch <= 0;
					stat <= 8;
				end
				8 : begin
					dir_latch <= 1;
					stat <= 0;
				end
				default : begin
					stat <= 0;
				end
			endcase
		end
endmodule