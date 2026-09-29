/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "core.h"
#include "PID.h"
#include "Emm_v5.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RXBUFFERSIZE  256

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rx_buffer[RXBUFFERSIZE + 1];	// Extra byte for the parser terminator
volatile uint16_t rx_len; 		//接收到的数据长度
volatile uint8_t recv_end_flag; 		//接收结束标志位
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_usart2_rx;


PID_t x_PID;
PID_t y_PID;

float bujin_angle1 = 0;
float Servo_angle2;
int cx, cy,flag;
int32_t cnt;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  x_PID.Kp = 2.3;
  x_PID.Ki = 0;
  x_PID.Kd = 5;
  y_PID.Kp = 6.3;
  y_PID.Ki = 0;
  y_PID.Kd = 3;
  cx = 80;
  cy = 60;
  Servo_angle2 = 180;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  float ccr = 500 + (Servo_angle2 * 2000 / 270);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ccr);

  HAL_TIM_Base_Start_IT(&htim2);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart2,rx_buffer,RXBUFFERSIZE);

  PID_Init(&x_PID);
  PID_Init(&y_PID);

  // PID_Init(&y_PID);
  // y_PID.Kp = 0.1;
  // y_PID.Ki = 0.1;
  // y_PID.Kd = 0.1;
  // __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1 ,1500);
  // __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2 ,1000);

  //开启UART1中断
  /* USART1 is transmit-only for the stepper driver. */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    // if(recv_end_flag == 1)
    // {
    //   // 打印解析后的cx/cy（正确输出数值）
    //   printf("cx: %d, cy: %d\r\n", cx, cy);
    //   // 打印原始接收数据（可选，调试用）
    //   // printf("原始数据：");
    //   // for(uint16_t i=0; i<rx_len; i++)
    //   // {
    //   //   printf("%c", rx_buffer[i]);
    //   // }
    //   printf("\r\n");
    //
    //   recv_end_flag = 0; // 清除标志位
    //   rx_len = 0;        // 重置接收长度
    // }

    // Tilt(cx,cy);
    // HAL_Delay(10); // 降低循环频率，避免占用过多CPU
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//核心控制
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)  // 确认是TIM2中断
  {
    if (recv_end_flag == 1)  // 仅当接收到OpenMV数据时执行
    {
        Tilt(cx, cy);          // 调用控制函数
        recv_end_flag = 0;     // 清除接收标志，避免重复处理
    }
  }
}

//接收摄像头获取的数据
void USART2_IRQHandler(void)
{
  uint32_t tmp_flag = 0;
  uint32_t temp = 0;

  // HAL库核心中断处理（必须保留）
  HAL_UART_IRQHandler(&huart2);

  // 检测IDLE空闲中断（接收完成标志）
  tmp_flag = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE);
  if((tmp_flag != RESET))
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart2); // 清除IDLE标志
    HAL_UART_DMAStop(&huart2);          // 停止DMA接收

    // 计算实际接收长度
    temp = __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
    rx_len = RXBUFFERSIZE - temp;

    // Parse only a complete x y flag triple from the OpenMV camera.
    if (rx_len > 0 && rx_len <= RXBUFFERSIZE)
    {
      int parsed_x, parsed_y, parsed_flag;
      rx_buffer[rx_len] = '\0';
      if (sscanf((const char *)rx_buffer, "%d %d %d",
                 &parsed_x, &parsed_y, &parsed_flag) == 3 &&
          parsed_x >= 0 && parsed_x < 320 &&
          parsed_y >= 0 && parsed_y < 240 &&
          (parsed_flag == 0 || parsed_flag == 1))
      {
        cx = parsed_x;
        cy = parsed_y;
        flag = parsed_flag;
        recv_end_flag = 1;
      }
    }

    // 清空缓冲区，重启DMA接收（关键：等待下一次数据）
    memset(rx_buffer, 0, RXBUFFERSIZE);
    HAL_UART_Receive_DMA(&huart2, rx_buffer, RXBUFFERSIZE);
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
#ifdef USE_FULL_ASSERT
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
