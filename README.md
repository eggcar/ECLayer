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
