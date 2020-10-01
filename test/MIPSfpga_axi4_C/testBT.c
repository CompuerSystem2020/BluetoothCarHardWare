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

char BT_uart_inbyte(void) 
{ 
	unsigned int RecievedByte; 
	while(!((*READ_IO(BT_UART_BASE + lsr) & 0x00000001)==0x00000001));
	RecievedByte = *READ_IO(BT_UART_BASE + rbr); 
	return (char)RecievedByte; 
}

void _mips_handle_irq(void* ctx, int reason) { 
	unsigned int value = 0;
	unsigned int period = 0;
	// *WRITE_IO(UART_BASE + ier) = 0x00000000; 
	// IER register. disable interrupts
	*WRITE_IO(IO_LEDR) =0xF00F; 
	//Display0xFFFFonLEDstoindicatereceive data from uart 
	delay();
	BT_rxData = BT_uart_inbyte(); 
	if (BT_rxData == 'w') { 
		round = 0; 
		*WRITE_IO(IO_LEDR)=0x1; 
		//Display0xFFFFonLEDstoindicatereceive data from uart 
		delay(); 
	} else if (BT_rxData == 's') { 
		round = 0;
	 	*WRITE_IO(IO_LEDR)=0x2; 
	 	//Display0xFFFFonLEDstoindicatereceive data from uart
	  	delay(); 
 	} else if (BT_rxData == 'q') {
   		*WRITE_IO(IO_LEDR)=0x4; 
   		//Display0xFFFFonLEDstoindicatereceive data from uart 
   		delay(); 
   	} else if (BT_rxData == 'e') {
    	*WRITE_IO(IO_LEDR)=0x8; 
    	//Display0xFFFFonLEDstoindicatereceive data from uart 
    	delay(); 
    } else if (BT_rxData == 'd') {
     	round = 0; 
     	*WRITE_IO(IO_LEDR) = 0x10; 
     	// Display 0xFFFF on LEDs to indicate receive data from uart 
     	delay(); 
 	}else if (BT_rxData == 'a') {
 	 	round = 0;
 	  	*WRITE_IO(IO_LEDR) = 0x20; 
 	  	// Display 0xFFFF on LEDs to indicate receive data from uart 
 	  	delay(); 
 	} else if (BT_rxData == '8') {
 	   	*WRITE_IO(IO_LEDR) = 0x20; 
 	   	// Display 0xFFFF on LEDs to indicate receive data from uart
 	    delay(); 
 	} else if (BT_rxData == 'h') {
 	    round = 0;
	    *WRITE_IO(IO_LEDR) = 0x20;
 	    // Display 0xFFFF on LEDs to indicate receive data from uart
 	    delay();     
 	} else { 
		round = 0;
 	    *WRITE_IO(IO_LEDR) = 0x40; 
 	    // Display 0xFFFF on LEDs to indicate receive data from uart
 	    delay(); 
 	}
	*WRITE_IO(IO_LEDR) =0xFFFF; 
	//Display0xFFFFonLEDstoindicatereceive data from uart 
	return; 
} 
