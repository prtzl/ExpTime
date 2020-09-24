# Expanded Time library for LL and HAL drivers for STM32

This library is used with both HAL and LL drivers for STM32. With LL drivers, there is no global tick counter and SyStick interrupt is disabled. When no HAL library is detected, this library using `expTime_init_ms` configures SysTick to overload at period of 1ms and trigger interrups that increment global variable `expTick`.

This library comes with example project located in folder `expTime_example` tested on STM32F407VG with HAL on all functions, as most people are familiar with it. Timer3 is already initialized for microsecond delays. You can toggle HAL and LL in Cube `Project manager` under `Advance Settings` Driver Selector.

## How to use this library

Copy library files to your project and add them to compiler paths.

To enable millis tick increment, go to file `stm32f4xx_it.c`. In function SysTick_Handler' and under section `USER CODE BEGIN SysTick_IRQn 0` add function call `expTime_IRQ`. Also in the beginning of this file under section 'USER CODE BEGIN Includes' add `#include "expTime.h`.

Include this header file wherever you need these functions. However you have to include it at least in `main.c` for initialization and `stm32abxx_it.c` as mentioned before. Call function `expTime_init` in your main under section `USER CODE BEGIN 2` right before while loop, before you call any other functions that use this library.

For delays you can use `expTime_delay_ms` for ms. Function works in HAL as well as it then uses HAL's 'uwTick' variable for tick reference. If you need microsecond delays, you can enable them in `expTime_Config.h`. Then you have to populate macros for either HAL or LL timer handle. Then you need to init an additional timer. 16 bit timers will for with delays and counts up to `65535` us, where as 32 bit timer will allow up to `4294967295` us or 71 min.

Microsecond timer configuration:
* Config timer to use internal clock,
* Config prescaler to ((APBx timer clock in MHz) -1 ), 84MHz APB1 -> prescaler = 83,
* Config to count up,
* Config auto-reload register to maximum it will allow -> 16 bit = 0xffff
* Everything else can be left on default

Any modifications, fixes and additions are welcome.