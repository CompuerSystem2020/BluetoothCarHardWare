#define WHEEL_BASE 0xB0700000

void pmd_helper(int x, int y)
{
    *WRITE_IO(WHEEL_BASE) = (x << 4) | y;
}

void forward(int speed)
{
	pmd_helper(0xD4,speed); // 1101 0100
}

void backward(int speed)
{
	pmd_helper(0x2B,speed); // 0010 1011
}

void turn_left(int speed)
{
    pmd_helper(0x66,speed); // 0110 0110
}

void turn_right(int speed)
{
	pmd_helper(0x99,speed); // 1001 1001
}

void pmd(char data, int *sp)
{	
	int sss = *sp;
	switch(data){
		case 'k': // speed up
			if(sss != 15)
				sss+=3;
			kprintf("k speed %d\n\r", sss);
			break;
		case 'm' : // speed down
			if(sss!=0)
				sss-=3;
			kprintf("m speed %d\n\r", sss);
			break;
		case 'w':
			forward(sss);	
			kprintf("forward\n\r");
			break;
		case 'a':
			turn_left(sss);
			kprintf("turn_left\n\r");
			break;
		case 'd':
			turn_right(sss);
			kprintf("turn_right\n\r");
			break;				
		case 's':
			backward(sss);
			kprintf("backward\n\r");
			break;
		case 't':
			forward(0);
			kprintf("stop\n\r");
    }
	*sp = sss;
}