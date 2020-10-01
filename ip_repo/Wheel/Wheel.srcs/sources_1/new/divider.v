module divider
(
	input clk,
	output clkk
);
	reg [31:0] cnt;
	reg flag = 0;
	always @(posedge clk)
		if (cnt < 25000)
			cnt <= cnt + 1;
		else
			begin
				cnt <= 0;
				flag = ~flag;
			end
	assign clkk = flag;
endmodule