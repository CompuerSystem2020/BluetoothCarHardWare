# 这里是state3-硬件部分的说明文档

## 主要任务

实现蓝牙驱动模块和马达驱动模块的硬件电路，同时做好相应的软件测试

## 配置需要

硬件部分：

1. vivado 2018.2
2. 开发板、下载器、马达、蓝牙模块、连接线和充电宝



软件部分：参考软件部分配置

## 蓝牙驱动模块实现步骤

### 1.修改block design

如下图，添加一个uart16550模块，同时添加TX输入端口和RX输出端口，连接好线路即可。


![bluetooth_block_design](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/bluetooth_block_design.png)

### 2.内存分配

如下图，我们为该模块的寄存器分配相应的内存地址，这里指定为0x10500000，因此对应的虚拟地址为0xB0500000，这将在软件部分进行使用。

![bluetooth_mem_assign](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/bluetooth_mem_assign.png)

### 3.端口分配

在端口方面我们使用Pmod Header JA里面的E18和G17端口，分别对应RX和TX。参考依据可见下图

![pmod_header](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/pmod_header.png)

这里我们选择使用type3，在实际使用时使用3，4针脚，针脚对应关系如下。

![pmod_pin](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/pmod_pin.png)

这里一定要搞清楚Pmod的针脚编号，在后面的马达驱动开发中要用到。具体的需要修改constraint文件。

### 4.重新生成写出bitstream

保存验证没问题后写出到新的bitstream，之后烧入开发板进行软件测试。

### 5.软件测试

具体的测试代码可以参考根目录下的test文件夹下的MIPSfpga_axi4_C文件夹中的main.c文件，此文件为对之前测试环境时所用文件的修改。

下面将拷贝出关键代码，并进行注释说明。

~~~c
#define BT_UART_BASE 0xB0501000 // 定义蓝牙模块对应的内存地址
void init_bluetooth(void) // 初始化相应的寄存器信息
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
char BT_uart_inbyte(void) // 从寄存器中读取数据， 每次只读取一个字符，并将该字符返回
{ 
	unsigned int RecievedByte; 
	while(!((*READ_IO(BT_UART_BASE + lsr) & 0x00000001)==0x00000001));
	RecievedByte = *READ_IO(BT_UART_BASE + rbr); 
	return (char)RecievedByte; 
}

char _mips_handle_irq(void* ctx, int reason) {

	char BT_rxData = 0;
	delay();
	BT_rxData = BT_uart_inbyte(); // 调用BT_uart_inbyte()获取一个字符
	*WRITE_IO(UART_BASE + thr) = BT_rxData; // 从终端写出该字符
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
//------------------
// main()
//------------------
int main() {
	char c = 0;
	int speed = 0;

	init_debug(); // 初始化openOCD相关的端口，也就是Pmod Header JB相连的硬件设备 可以不用管
	init_bluetooth(); // 初始化刚添加的uart16650的寄存器数据

	while(1) {
		c = _mips_handle_irq(0, 0);
//		pwm(c, &speed); // 这一句可以先不考虑，跟后面的马达驱动相关
	}
	return 0;
}
~~~

总的来说测试程序还是比较简单容易理解的。下面展示测试结果，通过蓝牙工具（手机端），连接到HC-06这个设备，依次发送 kwmasdt 字符进行测试。

![bluetooth_test](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/bluetooth_test.png)

可以看到蓝牙驱动部分运行正常。

## 马达驱动模块实现步骤

### 1.修改block design

首先在开始前先对L293D 驱动板进行一定的了解，该驱动板的核心芯片是74HCT595N 芯片，该芯片的具体的工作逻辑如下，在dir_clk时钟的驱动下，如果dir_enable使能（低电平有效）的情况下，将8个时钟周期内的从dir_serail端口读取8个串行信号，当8个信号读取完毕时，给出一个dir_latch信号进行所存，此8个信号代表的是将要控制4个马达的转动方向（正负方向，2个方向*4个马达），也就是说每一个所存的数据信号实际为8位，每次数据中有且只能有4个位为1，4个位为0。同时它将接收每个轮子的速度信号，这个速度由收到的信号中的占空比来决定。通过上面的了解我们可以做一下简单的设计。

1. 通过将系统的clk进行分频，产生一个频率稍微小点儿的dir_clk，从资料来看，通常要维持其为1000HZ
2. 由于我们需要实时地控制马达地转动方向，因此我们可以直接令dir_enable始终为0
3. dir_serial信号则需要我们通过配合使用dir_clk和CPU传送过来的数据产生，在将并行信号转换为8个周期的串行信号后，输出一个dir_latch信号，用作结束标志。
4. 由于存在四个马达需要进行控制，因此我们需要产生四个速度串行信号。通过控制01占空比来实现速度的调节。为了简化设计，我们假设四个轮子的速度均相同（不考虑正反旋转）。

通过上面的分析，我们可以首先将该模块实现出来，命名为WheelController模块，其具体的设计如下。

![wheel_controller](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/wheel_controller.png)

clk为开发板时钟，para为从CPU接收的数据，其中para[11:4] 代表这方向数据，para[3:0]代表这轮子的转速。dir_serial为输出的串行信号，dir_enable为使能信号，dir_latch为锁存信号，dir_clk为分频后的信号，wheel[3:0]中的每一个信号均有着与para[3:0]相关的相同的占空比信号。

由于要从CPU中读取并行的数据，因此我们这里使用了最简单的GPIO接口，只用其输出部分。如下图所示。

![pwm_block_design](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/pwm_block_design.png)

即CPU将数据写入到寄存器后，通过gpio_io_o[11:0]端输出到para[11:0]端，之后WheelController对该数据进行分析处理产生最终的控制信号。具体的可以去参考WheelController模块的Verilog实现。

### 2.内存分配

如下图，我们为gpio模块的寄存器分配相应的内存地址，这里指定为0x10700000，因此对应的虚拟地址为0xB0700000，这将在软件部分进行使用。

![pwm_mem_assign](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/pwm_mem_assign.png)

### 3.端口分配

此部分的端口分配将是一大难点。需要参考下图和前面的Pmod针脚对应图。

![L293D](https://github.com/CompuerSystem2020/BluetoothCarHardWare/blob/state3/pic/L293D.png)



通过资料我们可以看到要想控制四个轮子的速度，需要分别控制PWM2A、PWM2B、PWM0A、PWM0B这四个输入端口的输入信号，因此这四个信号需要对应我们的wheel[3:0]四个信号。虽然wheel[3:0]的信号都是一样的，但是我们还是做一下区分，方便后面功能的拓展。这里wheel[0]对应PWM2A，wheel[1]对应PWM0A，wheel[2]对应PWM2B，wheel[3]对应PWM0B。具体的关系表如下

| 端口       | 马达驱动板 | Pmod针脚 |
| :--------- | :--------- | :------- |
| dir_clk    | D4         | 2        |
| dir_serial | D8         | 8        |
| dir_enable | D7         | 7        |
| dir_latch  | D12        | 10       |
| wheel[0]   | D11(PWM2A) | 9        |
| wheel[1]   | D6(PWM0A)  | 1        |
| wheel[2]   | D3(PWM2B)  | 3        |
| wheel[3]   | D5(PWM0B)  | 4        |

按照上表即可将马达驱动板与n4卡发板连起来。

### 4.重新生成写出bitstream

保存验证没问题后写出到新的bitstream，之后烧入开发板进行软件测试。

### 5.软件测试

这一部分的软件测试跟蓝牙部分有点儿相似，下面给出核心代码。

~~~c
#define WHEEL_BASE 0xB0701000 // 马达GPIO模块的基地址
void pwm_helper(int x, int y) // 写出方向以及速度数据
{
    *WRITE_IO(WHEEL_BASE) = (x << 4) | y;
    delay();
}

void forward(int speed)
{
	pwm_helper(0xD4,speed); // 1101 0100 四个轮子均为正向
}

void backward(int speed)
{
	pwm_helper(0x2B,speed); // 0010 1011 四个轮子均为反向
}

void turn_left(int speed)
{
    pwm_helper(0x66,speed); // 0110 0110 左侧两个为反向，右侧两个为正向
}

void turn_right(int speed)
{
	pwm_helper(0x99,speed); // 1001 1001 左侧两个为正向，右侧两个为反向
}

void pwm(int data, int *speed) // 控制速度与运动方向
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
	char c = 0;
	int speed = 0;

	init_debug();
	init_bluetooth();

	while(1) {
		c = _mips_handle_irq(0, 0); // 蓝牙接收信息
		pwm(c, &speed); // 接收的信息用于控制小车
	}
	return 0;
}
~~~

如果配置正确的话，此时在手机端输入指令，马达将会开始正常运转。

至此，蓝牙小车的硬件部分全部完成。

## 感想

感觉这一部分还是比较复杂的，需要去看一些文档，查阅一部分的资料，即需要保证硬件是运行正确的，又要保证多个硬件之间的连接是正确的，最开始的时候搞不清楚，导致连接错误，即便硬件是正确的也运行不起来。另外软件部分也比较复杂，需要合理地控制端口内容的读写。虽然完成的过程中遇到了很多的坑，但总结下来还是相当有成就感的。