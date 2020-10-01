/*
 * main.c for microAptiv_UP MIPS core running on Nexys4DDR
 * Prints \n\rMIPSfpga\n\r out via UART.
 * Default baudrate 115200,8n1.
 * Also display a shifting 0xf on the LEDs
 */

#include "fpga.h"

#define inline_assembly()  asm("ori $0, $0, 0x1234")

#define CLK_FREQ 50000000

#define UART_BASE 0xB0401000	//With 1000 offset that axi_uart16550 has
#define BT_UART_BASE 0xB0501000
#define WHEEL_BASE 0xB0701000
#define rbr		0*4
#define ier		1*4
#define fcr		2*4
#define lcr		3*4
#define mcr		4*4
#define lsr		5*4
#define msr		6*4
#define scr		7*4

#define thr		rbr
#define iir		fcr
#define dll		rbr
#define dlm		ier

// 
void delay() {
	volatile unsigned int j;
	for (j = 0; j < (10000); j++) ;	// delay 
}

void init_bluetooth(void) 
{
	*WRITE_IO(BT_UART_BASE + lcr) = 0x00000080; 
	// LCR[7] is 1 
	delay(); 
	*WRITE_IO(BT_UART_BASE + dll) = 69; 
	// DLL msb. 115200 at 50MHz. Formula is Clk/16/baudrate. From axi_uart manual. 
	delay(); 
	*WRITE_IO(BT_UART_BASE + dlm) = 0x00000001; 
	// DLL lsb. 
	delay(); 
	*WRITE_IO(BT_UART_BASE + lcr) = 0x00000003; 
	// LCR register. 8n1 parity disabled 
	delay(); 
	*WRITE_IO(BT_UART_BASE + ier) = 0x00000001; 
	// IER register. disable interrupts 
	delay(); 
}
void init_debug(void) 
{
	*WRITE_IO(UART_BASE + lcr) = 0x00000080; // LCR[7]  is 1
	delay();
	*WRITE_IO(UART_BASE + dll) = 27; // DLL msb. 115200 at 50MHz. Formula is Clk/16/baudrate. From axi_uart manual.
	delay();
	*WRITE_IO(UART_BASE + dlm) = 0x00000000; // DLL lsb.
	delay();
	*WRITE_IO(UART_BASE + lcr) = 0x00000003; // LCR register. 8n1 parity disabled
	delay();
	*WRITE_IO(UART_BASE + ier) = 0x00000000; // IER register. disable interrupts
	delay();
}

char BT_uart_inbyte(void) 
{ 
	unsigned int RecievedByte; 
	while(!((*READ_IO(BT_UART_BASE + lsr) & 0x00000001)==0x00000001));
	RecievedByte = *READ_IO(BT_UART_BASE + rbr); 
	return (char)RecievedByte; 
}

char _mips_handle_irq(void* ctx, int reason) { 

	char BT_rxData = 0;
	delay();
	BT_rxData = BT_uart_inbyte(); 
	*WRITE_IO(UART_BASE + thr) = BT_rxData;
	delay(); 
	if (BT_rxData == 'k') { 
		*WRITE_IO(IO_LEDR)=0x1; 
		delay();
	} else if (BT_rxData == 'm') { 
	 	*WRITE_IO(IO_LEDR)=0x2; 
	  	delay(); 
 	} else if (BT_rxData == 'w') {
   		*WRITE_IO(IO_LEDR)=0x4; 
   		delay(); 
   	} else if (BT_rxData == 'a') {
    	*WRITE_IO(IO_LEDR)=0x8; 
    	delay(); 
    } else if (BT_rxData == 'd') {
     	*WRITE_IO(IO_LEDR) = 0x10; 
     	delay(); 
 	}else if (BT_rxData == 's') {
 	  	*WRITE_IO(IO_LEDR) = 0x20; 
 	  	delay(); 
 	} else if (BT_rxData == 't') {
 	   	*WRITE_IO(IO_LEDR) = 0x40; 
 	    delay(); 
 	} else { 
 	    *WRITE_IO(IO_LEDR) = 0x80; 
 	    delay();
 	}
 	*WRITE_IO(UART_BASE + thr) = '\n';
 	delay();
 	*WRITE_IO(UART_BASE + thr) = '\r';
 	delay();
	return BT_rxData; 
} 
void pmw_helper(int x, int y)
{
    *WRITE_IO(WHEEL_BASE) = (x << 4) | y;
    delay();
}

void forward(int speed)
{
	pmw_helper(0xD4,speed); // 1101 0100
}

void backward(int speed)
{
	pmw_helper(0x2B,speed); // 0010 1011
}

void turn_left(int speed)
{
    pmw_helper(0x66,speed); // 0110 0110
}

void turn_right(int speed)
{
	pmw_helper(0x99,speed); // 1001 1001
}

void pmw(int data, int *speed)
{	
	int sss = *speed;
	if (data == 'k') {
		if (sss != 15){
			sss += 3;
		}
	} else if (data == 'm') {
		if(sss != 0){
			sss -= 3;
		}
	} else if (data == 'w') {
		forward(sss);
	} else if (data == 'a') {
		turn_left(sss);
	} else if (data == 'd') {
		turn_right(sss);
	} else if (data == 's') {
		backward(sss);
	} else if (data == 't') {
		forward(0);
	}
	*speed = sss;
}
//------------------
// main()
//------------------
int main() {
	volatile unsigned int pushbutton, count = 0xF;
	volatile unsigned int j = 1;
	char c = 0;
	int speed = 0;

	init_debug();
	init_bluetooth();

	while(1) {
		c = _mips_handle_irq(0, 0);
		pmw(c, &speed);
	}
	return 0;
}

