# ECLayer
A stand-alone POSIX-like vfs abstract layer for ARM Cortex-M series

供ARM Cortex-M系列使用的独立的类POSIX虚拟文件抽象层

## What is ECLayer
ECLayer's purpose is to abstract the low-level operations for ARM Cortex-M based MCUs and CPUs, provides POSIX-like apis to embedded application developers.
Besides, there're some useful mechanisms for embedded development, such as list, lock, cross-compiler compat macros etc.

ECLayer的目的是将ARM Cortex-M系列的微处理器、微控制器的底层操作抽象成一套类POSIX的文件操作api，供应用层嵌入式开发人员使用。
此外，ECLayer还提供了一些嵌入式开发中有用的小机制，诸如链表、锁、跨编译器宏等等。

## Is ECLayer free?
Yes, ECLayer is totally free for personal and commercial use. You do not need to open-source your project. We're using Apache 2.0 license.

是的，个人和商业使用ECLayer完全免费，使用ECLayer不需要将你的项目开源。本项目遵循Apache2.0协议。

## Does ECLayer require an OS?
ECLayer is not designed to depend a specified OS, even more, you can use it in bare-metal(attention: realization of some drivers might require thread control).

ECLayer不需要依赖特定的操作系统，甚至可以在裸机应用中使用（注意：某些驱动程序的实现可能需要进程控制功能）

## Does ECLayer require any third-party packages?
In principle, ECLayer depends on ARM CMSIS only. Drivers of peripherals of any specified chip would possiblly depends on the chip's low-level library.

原则上，ECLayer只依赖于ARM的CMSIS库。特定型号芯片的外设驱动可能依赖于该芯片提供的底层操作库。

## Which development environment is supported?
Currently, we support armcc v5, armclang v6, gcc v4.4+ and iccarm compilers.
You should enable gnu99 standard when compiling your projects.

当前，我们支持armcc v5、armclang v6、gcc v4.4以上以及iccarm编译器。
编译工程时，请使能gnu99标准。

# Change Log

## 2020/05/09

### ECLayer Core
		Add lwip wrapper mode in ECLayer api. Support socket()/accept()/read()/write()...... BSD-like 
		socket api based on lwip v2.x(I didn't test v1.4.x but that might work), while shares the same 
		file descriptor domain with ECLayer POSIX api. We add a wrapper layer in file_des_s structure 
		to convert ECLayer fd number to lwip socket fd number.

		With this wrapper, you can use the same read()/write()/close()... api to ether serial ports or 
		tcp streams.

		在ECLayer API中添加了LWIP封装层。支持socket()、accept()、read()、write()...等BSD类socket api，基于
		LWIP v2.x版本（我没试过在V1.4.x上是否有效，按理说应该可以用）。与ECLayer的类POSIX API共享同一个文件描述
		符空间。在file_des_s结构体里添加了一个封装层，来支持ECLayer的fd号与lwip的fd号的互相转换。

		To use LWIP wrapper, you should import lwip in your project, then check ec_config.h to define
		_WITH_LWIP_SOCKET_WRAPPER = 1. Then in your lwip config file, make sure LWIP_COMPAT_SOCKETS = 0, 
		and LWIP_SOCKET = 1.

		使用LWIP封装层，你要先在你的工程里移植好LWIP。然后检查ec_config.h，将宏定义_WITH_LWIP_SOCKET_WRAPPER设置
		为1。然后在lwip的配置文件中，检查以下两个宏的定义：LWIP_COMPAT_SOCKETS = 0，LWIP_SOCKET = 1。

### STM32 U(S)ART driver:
		Fixed a bug imported in last commit that initialized a CMSIS-OS2 semaphore with wrong value.

		修正了上次提交中引入的一个初始化CMSIS-OS2 semaphore的参数错误。


## 2020/05/01
### ECLayer configuration
		Add a configuration in ec_config.h to enable/disable CMSIS-OS v2 api usage in ECLayer and ECDriver.
		Rewrite STM32 usart driver, solved a thread-safety issue.
		在ec_config.h中添加了一个配置项，可以使能/关闭在ECLayer和ECDriver中使用CMSIS-OS v2 API.
		重写了STM32的usart驱动，解决了一个线程安全性问题.
## 2019/09/16
### ECLayer core
		ec_try_lock_irqsave() now recovers the global IRQ status when failed to acquire the lock.
		ec_try_lock_irqsave()现在在获取锁失败的情况下，会自动恢复全局中断状态。

## 2019/08/21
### STM32 U(S)ART driver:
	Add support for tx/rx timestamp capture.
	添加了串口收发时间戳的捕获功能。

	Usage:
		First, you need to implement the timestamp API in systime_port.h
		Then, make sure _EN_USART_TIMESTAMP in ec_config.h is set to 1
		In your application code, after you called read/write method on a STM32 UART device, 
		you can call ioctl() function with command CMD_USART_GETREADTS and CMD_USART_GETWRITETS
		to get the receive/transmit timestamp. Tx timestamp is captured on the first byte of the
		last write() buffer filled into the U(S)ART hardware tx register. Rx timestamp is captured 
		on the first charactor read into the read() buffer when it arrived at the U(S)ART hardware
		rx register and we got into the interrupt handler.
		首先，你要实现systime_port.h中的时间戳API
		然后，确认ec_config.h文件中的_EN_USART_TIMESTAMP宏的值设置为1
		在应用代码中，调用STM32串口设备的read/write方法后，你可以使用ioctl()方法的CMD_USART_GETREADTS
		和CMD_USART_GETWRITETS命令来获取串口的接收/发送时间戳。发送时间戳是在上一个write()方法传入的第一个
		字节被填入串口发送寄存器的时刻被捕获的。接收时间戳是在上一个read()方法读取到的第一个字节被接收到串口
		接收寄存器中且我们进入了中断处理函数时捕获的。
	Application Example:
		timeStamp_t recv_ts, trans_ts;
		err = read(fd, (char *)buffer, 11);
		/* We get "Hello World" in the buffer here */
		if (err > 0) {
			ioctl(fd, CMD_USART_GETREADTS, (uint64_t) & (recv_ts));
		}
		/* The recv_ts is the timestamp when the charactor 'H' is received */

		/* We put "ECLayer UART TS example" in */
		err = write(fd, (char *)buffer, strlen(buffer));
		if (err > 0) {
			ioctl(fd, CMD_USART_GETWRITETS, (uint64_t) & (trans_ts));
		}
		/* The trans_ts is the timestamp when the charactor 'E' is transmitted */

### Timestamp portable API:
	Add systime_port.h that supplies an API used for timestamp capture. (Detail implementation needs to be filled in your own project)
	添加了systime_port.h文件，提供了用于时间戳捕获功能的API。（具体实现需要根据你的工程设计自行填充）
### Miscs:
	Some comment improved. Some typo fixed. Some compiler warnning handled.
	改进了一些注释。修改了个别拼写错误。处理了一些编译器警告。
