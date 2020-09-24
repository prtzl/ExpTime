#include "expTime.h"

// Global ms tick counter
#ifdef TIME_USE_HAL_TICK
#define expTick	uwTick
#else
uint32_t expTick;
#endif

/**
* @brief  Increments global gTick if LL driver is used
* @info		Call this function in SysTick_Handler in
* 				stm32abxx_it.c
**/
void expTime_IRQ(void)
{
#ifndef TIME_USE_HAL_TICK
	expTick++;
#endif
}

/**
* @brief  Enables SysTick interrupt
**/
void expTime_init_ms(void)
{
#ifndef TIME_USE_HAL_TICK
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  NVIC_EnableIRQ(SysTick_IRQn);
  NVIC_SetPriority(SysTick_IRQn, 0);
#endif
}

/**
* @brief  Returns global millis tick value
**/
uint32_t expTime_getTIck(void)
{
	return expTick;
}

/**
* @brief  Wait for ms milliseconds
* @param  ms milliseconds to wait
**/
void expTime_delay_ms(const uint32_t delay)
{
	uint32_t tick_start = expTime_getTIck();
	while((expTime_getTIck() -tick_start) < delay);
}

static uint32_t timeout_tick_start;
static uint32_t timeout_ms;
/**
* @brief  Starts timeout
**/
void expTime_timeout_start(const uint32_t timeout)
{
  timeout_tick_start = expTime_getTIck();
  timeout_ms = timeout;
}

/**
* @brief  Checks if timeout is reached
* @info   Call function timeout_start to update timeout tick start
**/
uint8_t expTime_timeout_check(void)
{
  // Return OK if timeout has been reached
  if(expTime_getTIck() > (timeout_tick_start +timeout_ms))
  {
    return 0;
  }
  // Timeout not reached
  else
  {
    return 1;
  }
}

/**
* @brief  Init microseconds timer
**/
void expTime_init_us(void)
{
#ifdef TIM_USE_LL
  LL_TIM_EnableCounter(TIM_HANDLE);
#elif defined(TIM_USE_HAL)
  HAL_TIM_Base_Start(&TIM_HANDLE);
#endif
}

/**
* @brief  Wait for us microseconds
* @param  us microseconds to wait
**/
void expTime_delay_us(uint32_t delay)
{
#if TIM_ARR_SIZE == 16
	// Check for 16bit timer to limit delay
	if(delay > (uint32_t)0xffff)
	{
		delay = (uint32_t)0xffff;
	}
#endif
#ifdef TIM_USE_LL
  LL_TIM_SetCounter(TIM_HANDLE, 0);
	while(LL_TIM_GetCounter(TIM_HANDLE) < delay);
#elif defined(TIM_USE_HAL)
  __HAL_TIM_SET_COUNTER(&TIM_HANDLE, 0);
  while(__HAL_TIM_GET_COUNTER(&TIM_HANDLE) < delay);
#endif
}

/**
* @brief  Init both ms and us time
**/
void expTime_init(void)
{
#ifndef TIME_USE_HAL_TICK
	expTime_init_ms();
#endif
#ifdef TIME_USE_USECONDS
	expTime_init_us();
#endif
}
