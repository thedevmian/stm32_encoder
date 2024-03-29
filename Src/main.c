/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "lcd_hd44780_i2c.h"

//volatile uint32_t timer_ms = 0;
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
void lcd_Start();
void Licznik(void);
void Pozycja(void);
void Czas(void);


int main(void)
{
  /* USER CODE BEGIN 1 */
	//uint8_t buffer_lcd [] = "Test\0"; 
	unsigned char *menu[5] = {" MENU: \0","Pozycja\0"," Czas \0","Licznik\0", "Informacje\0"};
	volatile unsigned long int stanA, ostatniStanA;
	unsigned char pozycja;
	pozycja = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USB_DEVICE_Init();
	lcd_Start();
  /* USER CODE BEGIN 2 */
	lcdCommand (LCD_CLEAR, LCD_PARAM_SET);
	//lcdPrintStr(buffer_lcd, sizeof(buffer_lcd));
  /* USER CODE END 2 */
	
	koniec:
	lcdDisplayClear();
	lcdPrintStr("-- \0",3);
	lcdPrintStr(menu[0],7);
	lcdPrintStr(" --\0",3);
	lcdSetCursorPosition(4,1);
	lcdPrintStr(menu[pozycja+1],7);

	ostatniStanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		stanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
		if(stanA != ostatniStanA)
		{
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) != stanA)
				pozycja++;
			else
				pozycja--;
			switch(pozycja)
			{
				case 0:
						lcdDisplayClear();
						lcdPrintStr("-- \0",3);
						lcdPrintStr(menu[0],7);
						lcdPrintStr(" --\0",3);
						lcdSetCursorPosition(4,1);
						lcdPrintStr(menu[pozycja+1],7);
						break;
				case 2:
						lcdDisplayClear();
						lcdPrintStr("-- \0",3);
						lcdPrintStr(menu[1],7);
						lcdPrintStr(" --\0",3);
						lcdSetCursorPosition(3,1);
						lcdPrintStr(menu[2],6);
						break;
				case 4:
						lcdDisplayClear();
						lcdPrintStr("-- \0",3);
						lcdPrintStr(menu[2],6);
						lcdPrintStr(" --\0",3);
						lcdSetCursorPosition(3,1);
						lcdPrintStr(menu[3], 7);
						break;
				case 6:
						lcdDisplayClear();
						lcdPrintStr("-- \0",3);
						lcdPrintStr(menu[3],7);
						lcdPrintStr(" --\0",3);
						lcdSetCursorPosition(1,1);
						lcdPrintStr(menu[4],10);
						break;
				case 8:
						lcdDisplayClear();
						lcdPrintStr("-- \0",3);
						lcdPrintStr(menu[4],10);
						lcdPrintStr(" --\0",3);
						
						break;
				default:
					break;
				
			}
		ostatniStanA = stanA;
		}	
		 
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
			switch (pozycja)
			{
					case 2: 
					Pozycja(); 
					pozycja = 0;
					goto koniec;			
				break;
				case 4:
					Czas();
					pozycja = 0;
					goto koniec;
				break;
				case 6:
					Licznik();
					pozycja = 0;
					goto koniec;
				break;
				case 8:
					lcd_Start();
					HAL_Delay(500);
					pozycja = 0;
					goto koniec;
				default:
				break;
			}
	}		
  /* USER CODE END WHILE */
		

		HAL_Delay(500);

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */ 

static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
void lcd_Start()
{
	
	lcdInit(&hi2c1, 0x27, 16,2);                   //inicjalizacja wysietlacza
  lcdDisplayClear();    												//wyczysc wyswietlacz
	lcdPrintStr("Projekt na STM32\0", 16);
	lcdSetCursorPosition(0,1);
	lcdPrintStr("Damian Dzialo\0", 13);
	while (1)
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
				break;
	}
	
	HAL_Delay(2500);
	lcdDisplayClear();
	return;
}

void Licznik(void)
{
	volatile unsigned long int licznik, stanA, ostatniStanA, ostatni_licznik;
	unsigned char licznikTekst[5] = {"0\0"};
	licznik = 0;
	HAL_Delay(500);
	ostatniStanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	
	lcdDisplayClear();
	lcdPrintStr("Licznik \0", 8);
	sprintf((char*)licznikTekst,"%2i\0",licznik);
	lcdSetCursorPosition(8,0);
	lcdPrintStr(licznikTekst,3);
	while(1)
	{
		ostatni_licznik = licznik;
		stanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
		if(stanA != ostatniStanA)
		{
		
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) != stanA)
				licznik++;
			else
				licznik--;


	lcdDisplayClear();
	lcdPrintStr("Licznik \0", 8);
	sprintf((char*)licznikTekst,"%2i\0",licznik);
	lcdSetCursorPosition(8,0);
	lcdPrintStr(licznikTekst,3);
		if(ostatni_licznik != licznik)
			{
				if (licznik > ostatni_licznik)
				{
					lcdSetCursorPosition(0,1);
					lcdPrintStr("Obrot w prawo\0", 13);
				}
				else
				{
					lcdSetCursorPosition(0,1);
					lcdPrintStr("Obrot w lewo\0", 12);
				}
			}
			
		ostatniStanA  = stanA;
		}
		
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
		{
		HAL_Delay(3000);
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
				{
					return;
				}
		}
	}
	
}

void Pozycja (void)
{
	volatile long int licznik, stanA, ostatniStanA, stopnie; 
  unsigned char stopnieTekst[5] = {"0\0"};
	licznik = 0;
	HAL_Delay(500);
	lcdDisplayClear();
	lcdPrintStr("Ustaw kat \0",10);
	lcdSetCursorPosition(0,1);
	lcdPrintStr("nachylenia \0", 10);

	while (1)
	{
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
		{	
			HAL_Delay(500);
			ostatniStanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
			lcdDisplayClear();
			lcdPrintStr("Wychylenie:\0",11);
			stopnie = 15 * (licznik/2);
			sprintf((char *)stopnieTekst,"%2i\0",stopnie);
			lcdSetCursorPosition(12,1);
			lcdPrintStr(stopnieTekst,3);
			lcdSetCursorPosition(15,1);
			lcdPrintChar(223);
			while (1) 
			{
				stanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
				if(stanA != ostatniStanA)
				{
					if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) != stanA)
						licznik++;
					else
						licznik--;
				lcdDisplayClear();
				lcdSetCursorPosition(0,1);
					lcdPrintStr("Wychylenie:\0",11);
				stopnie = 15 * (licznik/2);
				sprintf((char *)stopnieTekst,"%2i\0",stopnie);
				lcdSetCursorPosition(12,1);
				lcdPrintStr(stopnieTekst, 3);
				lcdSetCursorPosition(15,1);
				lcdPrintChar(223);
				if(stopnie == 360 || stopnie == -360)
				{
					licznik = 0;
					lcdDisplayClear();
					lcdSetCursorPosition(0,1);
					lcdPrintStr("Wychylenie:\0",11);
					stopnie = 15 * (licznik/2);
					sprintf((char *)stopnieTekst,"%2i\0",stopnie);
					lcdSetCursorPosition(12,1);
					lcdPrintStr(stopnieTekst,3);
					lcdSetCursorPosition(15,1);
					lcdPrintChar(223);
				}
				ostatniStanA  = stanA;
				}
				if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
					{
						HAL_Delay(3000);
						if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
							{
								return;
							}
					}
			}
		}
	}
}

void Czas(void)
{
	volatile unsigned long int stanA, ostatniStanA, czas;
	unsigned char czasTekst[3] = {"0\0"};
	czas = 0;
	HAL_Delay(500);
	lcdDisplayClear();
	lcdPrintStr("Ustaw czas pracy\0", 16);
	lcdSetCursorPosition(0,1);
	lcdPrintStr("w minutach\0", 10);
	while(1)
	{
	 if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
	 {
		 HAL_Delay(1000);
		 ostatniStanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
		
		 lcdDisplayClear();
		lcdPrintStr("Czas: \0", 5);
		sprintf((char *)czasTekst, "%2i/0", czas);
		lcdPrintStr(czasTekst,2);
		lcdPrintStr("m\0",1);
		 while(1)
		 {
			 stanA = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
		 if(ostatniStanA != stanA)
		 {
			 if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) != stanA)
				czas++;
			else
				czas--;
		lcdDisplayClear();
		lcdPrintStr("Czas: \0", 5);
		sprintf((char *)czasTekst, "%2i/0", czas);
		lcdPrintStr(czasTekst,2);
		lcdPrintStr("m\0",1);
		if(czas == -1)
		{
			czas = 0;
			lcdDisplayClear();
			lcdPrintStr("Czas: \0", 5);
			sprintf((char *)czasTekst, "%2i/0", czas);
			lcdPrintStr(czasTekst,2);
			lcdPrintStr("m\0",1);
		}
		 ostatniStanA = stanA;
	}
		 
		 if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
		{
			HAL_Delay(3000);
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)
				{
					return;
				}
		}
	 }
	}	 
}
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
