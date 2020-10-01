module pwm
(
	input clk,
	input [3:0] spd,
	output reg pwm_out
);
	reg [20:0] cnt = 0;
	reg [31:0] dc = 0;
	always @(spd)
		dc <= spd << 17;
	always @(posedge clk)
		begin
			cnt <= cnt + 1;
			if (cnt < dc)
				pwm_out <= 1;
			else
				pwm_out <= 0;
		end
endmodule