/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../User/Modules/Delay/delay.h"
#include "../User/Modules/Key/Key.h"
#include "../User/Modules/OLED/drv_OLED.h"
#include "../User/Modules/Buzzer/Buzzer.h"
#include "../User/Modules/LED/LED.h"
#include "../User/Modules/UltraSound/Ultrasound.h"
#include "../User/Modules/Motor/motor.h"
#include "../User/config.h"
#include "../User/Application/Car/Car.h"
#include "stdio.h"
#include "../User/Modules/OLED/Dwt.h"
#include "Kalman_filter/kalman_filter.h"
#include "../User/Modules/Track/track.h"
#include "../User/Modules/BatVolt/BatVolt.h"
#include "../User/Modules/Task_Scheduler/task_scheduler.h"
#include "../User/Application/Task/Task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart2, (uint8_t *) &ch, 1, HAL_MAX_DELAY);
    return ch;
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int8_t KeyNum;
int8_t Key_cnt = 1;

int8_t Ti_question = 0;
uint16_t main_cnt = 0; //主循环计数

// adc_buf[1]~[5] 对应5路循迹，adc_buf[0] 对应电池电压
uint16_t adc_buf[6];
static int xj_pwm = 3000;

// 定义环形缓冲区结构
#define UART_BUFFER_SIZE 64

typedef struct {
    uint8_t buffer[UART_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} uart_buffer_t;

// 全局环形缓冲区
uart_buffer_t uart2_rx_buffer = {0};



/* 接收缓冲区定义 */

uint8_t uart2_rx_data; // 暂存单次中断接收的1字节数据

uint8_t flag = 0;             // 状态标志

/* 串口1接收相关变量 */
#define UART1_RX_BUF_SIZE 64
uint8_t uart1_rx_buffer[UART1_RX_BUF_SIZE];
uint8_t uart1_rx_index = 0;
uint8_t uart1_rx_char;  // 单字节接收暂存


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

float motion6[7];       //加速度计和陀螺仪数据
float ypr[3];          //0为yaw（绕z轴转动），1为pitch（绕y轴转动），2为roll（绕x轴转动）

KalmanFilter2D *kf_yaw;            //卡尔曼滤波器

//yaw欧拉角滤波后结果
float yaw_filtered[2];

// 初始状态（yaw pitch roll）
float init_x[2] = {0.0f, 0.0f};

// 初始协方差
float init_P[4] =
        {1.0f, 0.0f,
         0.0f, 1.0f};

// 过程噪声矩阵（对角元素）反映“模型信任度”，值越小越相信模型
float Q[4] = {// Yaw噪声较大
        0.1f, 0.0f,
        0.0f, 0.01f, };

// 反映“传感器信任度”，值越小越相信测量
float R[4] = {
        0.5f, 0.0f,
        0.0f, 0.5f, };  // 先试 0.5，再微调




HAL_StatusTypeDef HAL_UART_STATUS;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void process_uart2_data(void);

void Mode_choose(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
    //pwm生成启动

    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);


    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
    //定时器中断开启
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);



    //uart2接受数据中断
    HAL_UART_Receive_IT(&huart2, &uart2_rx_data, 1);
    // 启动DMA搬运ADC数据
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, 6);

    delay_init();//里面包含imu计算时基nowtime
    dwt_init(72);
    //IMU_init(); //包含icm_42688初始化,里面启动了tim9的定时中断
    OLED_Init(); //
    Key_Init(); // 初始化按键
    OLED_Clear();
    LED_System_Init();
    Task_Init();
    Track_Init();



//    kf_yaw = Kalman2DInit(init_x, init_P, Q, R);
//    // 设置时间步长 (假设0.1秒)
//    Kalman2DSetDT(kf_yaw, 0.02f);

//任务管理初始化 注册任务
    Task_All_Init();
    //开启超声波测距
    Ultrasonic_Start_Measure();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    OLED_ShowString(1,1,"question:");
    OLED_ShowString(3,1,"pwm:");

    uint32_t last_tick = Flag_10ms_Cnt;

    /* USER CODE END 2 */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

        main_cnt++;
        // 赛题选择
        Mode_choose();
        //循迹pwm
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, xj_pwm);
        OLED_ShowString(3,1,"pwm:");//测试。；

        //根据赛题执行不同操作
        switch (Key_cnt)
        {
            case 1:

                Task_BatVolt_LED_Enable(true) ;
                Task_line_following_Pid_Enable(false) ;
                Task_Ultrasonund_Enable(true);
                Task_Music_Enable(false) ;
                Buzzer_Stop_Song();
                Task_OLED_Enable(true) ;
                Task_Car_Pidcontrol_Enable(false);
                Car_stop();    // 禁用小车控制时清零残留速度

                break;
            case 2:

                Task_BatVolt_LED_Enable(true) ;
                Task_line_following_Pid_Enable(true) ;
                Task_Ultrasonund_Enable(true);
                Task_Music_Enable(true) ;
                Task_OLED_Enable(true) ;
                Task_Car_Pidcontrol_Enable(true);

                break;
            case 3:

                Task_BatVolt_LED_Enable(true) ;
                Task_line_following_Pid_Enable(false) ;
                Task_Ultrasonund_Enable(true);
                Task_Music_Enable(true) ;
                Task_OLED_Enable(true) ;
                Task_Car_Pidcontrol_Enable(true);

                break;
            case 4:
                Task_BatVolt_LED_Enable(true) ;
                Task_line_following_Pid_Enable(false) ;
                Task_Ultrasonund_Enable(false);
                Task_Music_Enable(false) ;
                Buzzer_Stop_Song();
                Task_OLED_Enable(true) ;
                Task_Car_Pidcontrol_Enable(true);


                break;
            default:
                Car_stop();
                Buzzer_OFF();

                break;
        }

        if (Flag_10ms_Cnt != last_tick)
        {
            last_tick = Flag_10ms_Cnt;
            Task_Scheduler_Run();
        }
        // 控制小车
        Car_conDirection(Motor[0].Speed_out, Motor[1].Speed_out);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void Mode_choose(void)
{
    // 获取按键值
    KeyNum = Key_GetNum();

    // 处理按键事件
    if(KeyNum == 0) // KEY0按下
    {
        Key_cnt = (Key_cnt)%4 + 1;
    }

}





void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if(htim == &htim2)//htim2 10ms 100HZ进一次中断；
    {

        Flag_10ms_Cnt++;

        // 按键消抖（10ms 中断一次，计数3次=30ms）
        Key_Update();

        //更新imu数据
//        IMU_getYawPitchRoll(ypr);
//        IMU_TT_getgyro(motion6);
//
//        Kalman2DUpdateSeparate(kf_yaw, ypr[0], motion6[3]);
//        Kalman2DGetState(kf_yaw,yaw_filtered);

    }
    if(htim == &htim3)
    {
        // 调用结构体内部呼吸方法
        Led_Group[LED_R_GROUP].Breath_Scan(&Led_Group[LED_R_GROUP]);
        Led_Group[LED_L_GROUP].Breath_Scan(&Led_Group[LED_L_GROUP]);

    }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if (huart == &huart2) {
        // 将数据存入环形缓冲区

        uint16_t next_head = (uart2_rx_buffer.head + 1) % UART_BUFFER_SIZE;
        if (next_head != uart2_rx_buffer.tail) {
            uart2_rx_buffer.buffer[uart2_rx_buffer.head] = uart2_rx_data;
            uart2_rx_buffer.head = next_head;
        }

        HAL_UART_Receive_IT(&huart2, &uart2_rx_data, 1);
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
