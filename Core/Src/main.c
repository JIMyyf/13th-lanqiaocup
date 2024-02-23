/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "lcd.h"
#include "stdio.h"
#include "led.h"
#include "stdbool.h"
#include "delay.h"
#include "AT24C02.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define XNumerAddress 0x00
#define YNumerAddress 0x01
#define XPriceAddress 0x02
#define YPriceAddress 0x03
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int mode=0;char text[30];
typedef struct SHOPPINGCENTER
{
    int number;
    double price;
    int buy_number;
} members;
members member[2];
int counter = 0;
bool found = false;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1)
        if((USART1_RECEIVE_STATE&0x8000)==0)
            if (USART1_RECEIVE_STATE&0x4000){
                if (USART1_RECEIVE_DATA[0]!=0x0a) USART1_RECEIVE_STATE=0;
                else USART1_RECEIVE_STATE|=0x8000;}
            else{
                if (USART1_RECEIVE_DATA[0]==0x0d) USART1_RECEIVE_STATE|=0x4000;
                else{
                    USART1_RECEIVE_ARRAY[USART1_RECEIVE_STATE&0X3FFF]=USART1_RECEIVE_DATA[0];
                    USART1_RECEIVE_STATE++;
                    if (USART1_RECEIVE_STATE>(USART1_RECEIVE_LENGTH-1)) USART1_RECEIVE_STATE=0;
                }
            }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
    LCD_Init();LCD_Clear(Black);LCD_SetBackColor(Black);LCD_SetTextColor(White);
    delay_init(80);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_2);
    HAL_UART_Receive_IT(&huart1,&USART1_RECEIVE_DATA,USART1_RECEIVE_BUUFER_SIZE);
    HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  member[0].number=10;
    member[0].price=1.0;
    member[0].buy_number=0;
    member[1].number=10;
    member[1].price=1.0;
    member[1].buy_number=0;
  led_init();
  AT24C02_Write(XNumerAddress,member[0].number);
    delay_ms(5);
    AT24C02_Write(YNumerAddress,member[1].number);
    delay_ms(5);
    AT24C02_Write(XPriceAddress,member[0].price);
    delay_ms(5);
    AT24C02_Write(YPriceAddress,member[1].price);
    delay_ms(5);

  while(1){
    if (USART1_RECEIVE_STATE & 0x8000){
        if (USART1_RECEIVE_ARRAY[0]=='?') printf("%.1lf %.1lf\r\n",member[0].price,member[1].price);
        USART1_RECEIVE_STATE=0;
      }
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0){
        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0);
        mode++;
        if(mode==3) mode=0;
    }else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0){
        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0);
        switch(mode){
        case 0:
            member[0].buy_number++;
            break;
        case 1:
            member[0].price+=0.1;
            AT24C02_Write(XPriceAddress,member[0].price);
            break;
        case 2:
            member[0].number++;
            AT24C02_Write(XNumerAddress,member[0].number);
            break;
        }
    }else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0){
        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0);
        switch(mode){
        case 0:
            member[1].buy_number++;
            break;
        case 1:
            member[1].price += 0.1;
            AT24C02_Write(YPriceAddress,member[1].price);
            break;
        case 2:
            member[1].number++;
            AT24C02_Write(YNumerAddress,member[1].number);
            break;
        }
    }else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0){
        while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0);
        found=true;
        printf("X:%d, Y:%d, Z:%.1lf\r\n",member[0].buy_number,member[1].buy_number,
               member[0].buy_number*member[0].price+member[1].buy_number*member[1].price);
        member[0].number-=member[0].buy_number;
        member[0].buy_number=0;
        member[1].number-=member[1].buy_number;
        member[1].buy_number=0;
        AT24C02_Write(XNumerAddress,member[0].number);
        AT24C02_Write(YNumerAddress,member[1].number);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM6){
        if (mode==0){
            LCD_DisplayStringLine(Line2, (u8 *)"         SHOP   ");
            sprintf(text, "      X:%d   ", member[0].buy_number);
            LCD_DisplayStringLine(Line4, (u8 *)text);
            sprintf(text, "      Y:%d   ", member[1].buy_number);
            LCD_DisplayStringLine(Line5, (u8 *)text);
        }else if(mode==1){
            LCD_DisplayStringLine(Line2, (u8 *)"         PRICE  ");
            sprintf(text, "      X:%.1lf    ", member[0].price);
            LCD_DisplayStringLine(Line4, (u8 *)text);
            sprintf(text, "      Y:%.1lf    ", member[1].price);
            LCD_DisplayStringLine(Line5, (u8 *)text);
        }else{
            LCD_DisplayStringLine(Line2, (u8 *)"          REG   ");
            sprintf(text, "      X:%d   ", member[0].number);
            LCD_DisplayStringLine(Line4, (u8 *)text);
            sprintf(text, "      Y:%d   ", member[1].number);
            LCD_DisplayStringLine(Line5, (u8 *)text);
        }
        
        if(member[0].number==0&&member[1].number==0) led_Toggle(2);

        if(found==true){
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 30);
            counter++;
            led_ON(1);
            if (counter==50){
                found=false;
                led_OFF(1);
            }
        }
        else __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 5);
        HAL_TIM_Base_Start_IT(&htim6);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
