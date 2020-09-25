/* This library is used with both HAL and LL drivers for STM32.
 * With LL drivers, there is no global tick counter and SyStick
 * interrupt is disabled. When no HAL library is detected, this
 * library using 'expTime_init_ms' configures SysTick to overload
 * at period of 1ms and trigger interrups.
 *
 * This library comes with example project used on STM32F407VG with
 * HAL on all functions, as most people are familiar with it. Timer3 is
 * initialized for microsecond delays.
 *
 * To enable millis tick increment, go to file 'stm32f4xx_it.c'.
 * In function 'SysTick_Handler' and under section
 * 'USER CODE BEGIN SysTick_IRQn 0' add function call "expTime_IRQ"
 * Also in the beginning of this file under
 * section 'USER CODE BEGIN Includes' add '#include "expTime.h"
 *
 * Include this header file wherever you need these functions.
 * However you have to include it at least in main.c for initialization
 * and 'stm32abxx_it.c' as mentioned before.
 * Call function "expTime_init" in your main under section
 * "USER CODE BEGIN 2" right before while loop, before you
 * call any other functions that use this library.
 *
 * For delays you can use 'expTime_delay_ms' for ms. Function works in HAL as well
 * as it then uses HAL's 'uwTick' variable for tick reference.
 * If you need microsecond delays, you can enable them in 'expTime_Config.h'.
 * Then you have to populate macros for either HAL or LL timer handle.
 * Then you need to init an additional timer.
 * 16 bit timers will for with delays and counts up to 65535 us, 
 * where as 32 bit timer will allow up to 4294967295 us or 71 min.
 * 
 * Config timer to use internal clock,
 * Config prescaler to ((APBx timer clock in MHz) -1 ), 84MHz APB1 -> prescaler = 83,
 * Config to count up,
 * Config auto-reload register to maximum it will allow -> 16 bit = 0xffff
 * Everything else can be left on default
 *
 * source: https://github.com/SausageCode/nekinekineki
 * written by Mate Blagsic (@SausageCode)
 */

#ifndef TIME_H
#define TIME_H

#include "main.h"
#include "expTime_Config.h"

// Choose between HAL and LL for millis
#ifdef HAL_MODULE_ENABLED
#define TIME_USE_HAL_TICK
#else
extern uint32_t expTick;
#endif

/* US timer */
#ifdef  TIME_USE_USECONDS
/* Choose TIMER for us driver */
#ifdef HAL_TIM_MODULE_ENABLED
#define TIM_USE_HAL
/* If HAL is used for your us timer, choose the right timer handle 'htimx' */
extern TIM_HandleTypeDef htim3;
#define TIM_HANDLE	TIME_HAL_HANDLE
#else
#define TIM_USE_LL
/* If LL is used for your us timer, choose the right timer macro 'TIMx' */
#define TIM_HANDLE	TIME_LL_MACRO
#endif
#endif

/* Function prototypes */
// Tick incr function
void expTime_IRQ(void);
// ms functions
void expTime_init_ms(void);
uint32_t expTime_getTick(void);
void expTime_delay_ms(const uint32_t delay);
// Timeout functions
void expTime_timeout_start(const uint32_t timeout);
uint8_t expTime_timeout_check(void);
// us functions
void expTime_init_us(void);
void expTime_delay_us(uint32_t delay);
// Init
void expTime_init(void);

#endif
