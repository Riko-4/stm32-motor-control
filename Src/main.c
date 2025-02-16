  /* USER CODE BEGIN Header */
  /**
    ******************************************************************************
    * @file           : main.c
    * @brief          : Main program body
    ******************************************************************************
    */
  /* USER CODE END Header */
  /* Includes ------------------------------------------------------------------*/
  #include "main.h"
  #include <stdio.h>

  /* Private variables ---------------------------------------------------------*/
  TIM_HandleTypeDef htim1;
  TIM_HandleTypeDef htim2;
  TIM_HandleTypeDef htim3;
  TIM_HandleTypeDef htim4; // Timer for Ultrasonic Sensor

  /* USER CODE BEGIN PV */
  uint32_t IC_Val1 = 0;
  uint32_t IC_Val2 = 0;
  uint8_t Is_First_Captured = 0;
  uint32_t Distance = 0;
  /* USER CODE END PV */

  /* Private function prototypes -----------------------------------------------*/
  void SystemClock_Config(void);
  static void MX_GPIO_Init(void);
  static void MX_TIM1_Init(void);
  static void MX_TIM2_Init(void);
  static void MX_TIM3_Init(void);
  static void MX_TIM4_Init(void); // Timer for Ultrasonic Sensor
  void delay(uint16_t time);
  void HCSR04_Read(void);

  /* USER CODE BEGIN 0 */

  /* Delay Function using Timer */
  void delay(uint16_t time) {
      __HAL_TIM_SET_COUNTER(&htim4, 0);
      while (__HAL_TIM_GET_COUNTER(&htim4) < time);
  }

  /* Ultrasonic Sensor Measurement */
  void HCSR04_Read(void) {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  // Trigger HIGH
      delay(10); // 10us delay
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // Trigger LOW

      __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);  // Enable interrupt to capture Echo pulse
  }

  /* Capture Callback Function for Timer Interrupt */
  void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
      if (htim->Instance == TIM4) {  // If the interrupt is triggered by TIM4
          if (Is_First_Captured == 0) {
              IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
              Is_First_Captured = 1;
              __HAL_TIM_SET_COUNTER(htim, 0);
          } else {
              IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
              __HAL_TIM_SET_COUNTER(htim, 0);

              if (IC_Val2 > IC_Val1) {
                  Distance = (IC_Val2 - IC_Val1) * 0.034 / 2;
              }
              Is_First_Captured = 0;
              __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);
          }
      }
  }

  /* USER CODE END 0 */

  /**
    * @brief  The application entry point.
    */
  int main(void) {
      /* MCU Configuration */
      HAL_Init();
      SystemClock_Config();

      /* Initialize all configured peripherals */
      MX_GPIO_Init();
      MX_TIM1_Init();
      MX_TIM2_Init();
      MX_TIM3_Init();
      MX_TIM4_Init();

      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
      HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
      HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
      HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1); // Start input capture interrupt for ultrasonic sensor

      uint16_t speed_motor1 = 500;
      uint16_t speed_motor2 = 750;
      uint16_t speed_motor3 = 250;

      /* Infinite loop */
      while (1) {
          HCSR04_Read(); // Measure distance

          if (Distance > 10) { // If obstacle is farther than 10cm
              HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
              HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
              HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);

              for (uint16_t duty = 0; duty <= 999; duty += 50) {
                  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty);
                  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
                  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
                  HAL_Delay(100);
              }
          } else { // If obstacle detected within 10cm, stop motors
              __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
              __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
              __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
          }

          HAL_Delay(1000);
      }
  }

  /**
    * @brief TIM4 Initialization Function (For Ultrasonic Sensor Timing)
    */
  static void MX_TIM4_Init(void) {
      TIM_IC_InitTypeDef sConfigIC = {0};

      htim4.Instance = TIM4;
      htim4.Init.Prescaler = 71;
      htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
      htim4.Init.Period = 65535;
      htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
      htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
      if (HAL_TIM_IC_Init(&htim4) != HAL_OK) {
          Error_Handler();
      }

      sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
      sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
      sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
      sConfigIC.ICFilter = 0;
      if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK) {
          Error_Handler();
      }
  }

  /**
    * @brief GPIO Initialization Function (Added Ultrasonic Sensor GPIOs)
    */
  static void MX_GPIO_Init(void) {
      GPIO_InitTypeDef GPIO_InitStruct = {0};

      __HAL_RCC_GPIOA_CLK_ENABLE();

      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_8, GPIO_PIN_RESET);

      GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_8;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_9; // Echo Pin
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
