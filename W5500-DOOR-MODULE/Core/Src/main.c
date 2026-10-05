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
#include "wizchip_port.h"
#include "MQTT_Example.h"

#include <stdio.h>
#include <string.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RF_DATA_Pin               GPIO_PIN_8
#define RF_DATA_GPIO_Port         GPIOA

#define RF_BIT_US                 300
#define RF_PAUSE_US               10836
#define RF_FRAME_COUNT            5

#define TOPIC_CMD                 "WO/08/mezanin/micro/pintu_depan/cmnd/power"
#define TOPIC_IP                  "WO/08/mezanin/micro/pintu_depan/IP"
#define TOPIC_LWT                 "WO/08/mezanin/micro/pintu_depan/availability"

/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/

ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;


/* USER CODE BEGIN PV */

/*
 * RF CODE A
 */
static const char *RF_CODE_A[RF_FRAME_COUNT] =
{
    "110111010001000111011101000111010001000100011101110100011101110111011101101000100010001000111010",

    "110111010001000111011101000111010001000100011101110100011101110111011101101000100010001000111010",

    "110111010001000111011101000111010001000100011101110100011101110111011101101000100010001000111010",

    "110111010001000111011101000111010001000100011101110100011101110111011101101000100010001000111010",

    "110111010001000111011101000111010001000100011101110100011101110111011101101000100010001000111010"
};


/*
 * RF CODE B
 */
static const char *RF_CODE_B[RF_FRAME_COUNT] =
{
    "1110111010001000111011101000111010001000100011101110100011101110111011101110100010001000111010001",

    "1110111010001000111011101000111010001000100011101110100011101110111011101110100010001000111010001",

    "1110111010001000111011101000111010001000100011101110100011101110111011101110100010001000111010001",

    "1110111010001000111011101000111010001000100011101110100011101110111011101110100010001000111010001",

    "1110111010001000111011101000111010001000100011101110100011101110111011101110100010001000111010001"
};

/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);


/* USER CODE BEGIN PFP */

static void DWT_Delay_Init(void);
static void DWT_Delay_us(uint32_t us);

static void RF_SendBit(uint8_t bit);
static void RF_SendPattern(const char *pattern);

void RF_SendA(void);
void RF_SendB(void);

void RF_ProcessCommand(
    const char *topic,
    const char *payload
);

void led_Control(int state);

/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/*
 * DWT delay initialization
 */
static void DWT_Delay_Init(void)
{
    CoreDebug->DEMCR |=
        CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;

    DWT->CTRL |=
        DWT_CTRL_CYCCNTENA_Msk;
}


/*
 * Microsecond delay
 */
static void DWT_Delay_us(uint32_t us)
{
    uint32_t start =
        DWT->CYCCNT;

    uint32_t cycles =
        us *
        (HAL_RCC_GetHCLKFreq() / 1000000U);

    while (
        (DWT->CYCCNT - start) <
        cycles
    )
    {
    }
}


/*
 * RF send bit
 */
static void RF_SendBit(uint8_t bit)
{
    if (bit)
    {
        RF_DATA_GPIO_Port->BSRR =
            RF_DATA_Pin;
    }
    else
    {
        RF_DATA_GPIO_Port->BRR =
            RF_DATA_Pin;
    }

    DWT_Delay_us(
        RF_BIT_US
    );
}


/*
 * RF send pattern
 */
static void RF_SendPattern(
    const char *pattern
)
{
    while (*pattern)
    {
        RF_SendBit(
            *pattern == '1'
        );

        pattern++;
    }

    RF_DATA_GPIO_Port->BRR =
        RF_DATA_Pin;

    DWT_Delay_us(
        RF_PAUSE_US
    );
}


/*
 * Send RF CODE A
 */
void RF_SendA(void)
{
    printf(
        "RF: Sending CODE A\r\n"
    );

    for (
        uint8_t i = 0;
        i < RF_FRAME_COUNT;
        i++
    )
    {
        RF_SendPattern(
            RF_CODE_A[i]
        );
    }

    RF_DATA_GPIO_Port->BRR =
        RF_DATA_Pin;

    printf(
        "RF: CODE A complete\r\n"
    );

    HAL_GPIO_TogglePin(
        GPIOC,
        GPIO_PIN_13
    );
}


/*
 * Send RF CODE B
 */
void RF_SendB(void)
{
    printf(
        "RF: Sending CODE B\r\n"
    );

    for (
        uint8_t i = 0;
        i < RF_FRAME_COUNT;
        i++
    )
    {
        RF_SendPattern(
            RF_CODE_B[i]
        );
    }

    RF_DATA_GPIO_Port->BRR =
        RF_DATA_Pin;

    printf(
        "RF: CODE B complete\r\n"
    );
}


/*
 * Process MQTT command
 *
 * WO/08/Door/cmd
 *
 * Payload:
 *
 * 1 = RF CODE A
 * 0 = RF CODE B
 */
void RF_ProcessCommand(
    const char *topic,
    const char *payload
)
{
    printf(
        "RF COMMAND\r\n"
    );

    printf(
        "Topic   : %s\r\n",
        topic
    );

    printf(
        "Payload : %s\r\n",
        payload
    );


    if (
        strcmp(
            payload,
            "on"
        ) == 0
    )
    {
        RF_SendA();

        return;
    }


    if (
        strcmp(
            payload,
            "off"
        ) == 0
    )
    {
        RF_SendB();

        return;
    }


    printf(
        "RF: Invalid command\r\n"
    );
}


/*
 * LED control
 *
 * PC13 active LOW
 */
void led_Control(int state)
{
    if (state)
    {
        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_13,
            GPIO_PIN_RESET
        );
    }
    else
    {
        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_13,
            GPIO_PIN_SET
        );
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

    HAL_Init();


    /* USER CODE BEGIN Init */
    /* USER CODE END Init */


    SystemClock_Config();


    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */


    /* Initialize all configured peripherals */

    MX_GPIO_Init();

    MX_USART1_UART_Init();

    MX_ADC1_Init();

    MX_TIM2_Init();

    MX_SPI1_Init();

    MX_I2C1_Init();


    /* USER CODE BEGIN 2 */

    printf(
        "\r\n================================\r\n"
    );

    printf(
        " STM32F103 MQTT + RF SYSTEM\r\n"
    );

    printf(
        "================================\r\n"
    );


    /*
     * Start TIM2 PWM
     */
    HAL_TIM_PWM_Start(
        &htim2,
        TIM_CHANNEL_1
    );


    /*
     * ADC calibration
     */
    HAL_ADCEx_Calibration_Start(
        &hadc1
    );


    /*
     * Initialize DWT
     */
    DWT_Delay_Init();


    /*
     * RF output LOW
     */
    HAL_GPIO_WritePin(
        RF_DATA_GPIO_Port,
        RF_DATA_Pin,
        GPIO_PIN_RESET
    );


    /*
     * Initialize W5500
     */
    printf(
        "W5500: Initializing...\r\n"
    );

    W5500_Init();

    printf(
        "W5500: Initialization complete\r\n"
    );


    /*
     * Initialize MQTT TCP connection
     */
    if (
        mqtt_network_init() != 0
    )
    {
        printf(
            "MQTT: Network initialization failed\r\n"
        );
    }
    else
    {
        /*
         * Connect MQTT broker
         */
        if (
            mqtt_connect_broker() == 0
        )
        {
            /*
             * Subscribe CMD FIRST
             */
            if (
                mqtt_subscribe(
                    TOPIC_CMD
                ) == 0
            )
            {
                printf(
                    "MQTT: CMD subscription OK\r\n"
                );


                /*
                 * Publish STM32 IP
                 */
                mqtt_publish_ip(
                    TOPIC_IP
                );


                /*
                 * Give broker time to finish subscribe
                 */
                HAL_Delay(100);


                /*
                 * Automatic test command
                 *
                 * 1 = RF CODE A
                 *
                 * This is NOT retained.
                 */
                printf(
                    "MQTT: Publishing automatic CMD = 1\r\n"
                );

                mqtt_publish(
                    TOPIC_CMD,
                    "1"
                );
            }
            else
            {
                printf(
                    "MQTT: CMD subscription FAILED\r\n"
                );
            }


            printf(
                "MQTT: System ready\r\n"
            );
        }
        else
        {
            printf(
                "MQTT: Broker connection failed\r\n"
            );
        }
    }


    /* USER CODE END 2 */


    /* Infinite loop */

    /* USER CODE BEGIN WHILE */

    while (1)
    {
        /*
         * MQTT processing
         */
        mqtt_yield();


        /*
         * Small delay
         */
        HAL_Delay(10);


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
    RCC_OscInitTypeDef RCC_OscInitStruct =
        {0};

    RCC_ClkInitTypeDef RCC_ClkInitStruct =
        {0};

    RCC_PeriphCLKInitTypeDef PeriphClkInit =
        {0};


    /*
     * Initializes the RCC Oscillators
     */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSE;

    RCC_OscInitStruct.HSEState =
        RCC_HSE_ON;

    RCC_OscInitStruct.HSEPredivValue =
        RCC_HSE_PREDIV_DIV1;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_ON;

    RCC_OscInitStruct.PLL.PLLSource =
        RCC_PLLSOURCE_HSE;

    RCC_OscInitStruct.PLL.PLLMUL =
        RCC_PLL_MUL9;


    if (
        HAL_RCC_OscConfig(
            &RCC_OscInitStruct
        ) != HAL_OK
    )
    {
        Error_Handler();
    }


    /*
     * Initializes the CPU, AHB and APB buses
     */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;


    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_PLLCLK;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV2;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;


    if (
        HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_2
        ) != HAL_OK
    )
    {
        Error_Handler();
    }


    PeriphClkInit.PeriphClockSelection =
        RCC_PERIPHCLK_ADC;

    PeriphClkInit.AdcClockSelection =
        RCC_ADCPCLK2_DIV6;


    if (
        HAL_RCCEx_PeriphCLKConfig(
            &PeriphClkInit
        ) != HAL_OK
    )
    {
        Error_Handler();
    }
}


/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig =
        {0};


    hadc1.Instance =
        ADC1;

    hadc1.Init.ScanConvMode =
        ADC_SCAN_DISABLE;

    hadc1.Init.ContinuousConvMode =
        DISABLE;

    hadc1.Init.DiscontinuousConvMode =
        DISABLE;

    hadc1.Init.ExternalTrigConv =
        ADC_SOFTWARE_START;

    hadc1.Init.DataAlign =
        ADC_DATAALIGN_RIGHT;

    hadc1.Init.NbrOfConversion =
        1;


    if (
        HAL_ADC_Init(
            &hadc1
        ) != HAL_OK
    )
    {
        Error_Handler();
    }


    sConfig.Channel =
        ADC_CHANNEL_0;

    sConfig.Rank =
        ADC_REGULAR_RANK_1;

    sConfig.SamplingTime =
        ADC_SAMPLETIME_1CYCLE_5;


    if (
        HAL_ADC_ConfigChannel(
            &hadc1,
            &sConfig
        ) != HAL_OK
    )
    {
        Error_Handler();
    }
}


/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{
    hi2c1.Instance =
        I2C1;

    hi2c1.Init.ClockSpeed =
        100000;

    hi2c1.Init.DutyCycle =
        I2C_DUTYCYCLE_2;

    hi2c1.Init.OwnAddress1 =
        0;

    hi2c1.Init.AddressingMode =
        I2C_ADDRESSINGMODE_7BIT;

    hi2c1.Init.DualAddressMode =
        I2C_DUALADDRESS_DISABLE;

    hi2c1.Init.OwnAddress2 =
        0;

    hi2c1.Init.GeneralCallMode =
        I2C_GENERALCALL_DISABLE;

    hi2c1.Init.NoStretchMode =
        I2C_NOSTRETCH_DISABLE;


    if (
        HAL_I2C_Init(
            &hi2c1
        ) != HAL_OK
    )
    {
        Error_Handler();
    }
}


/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{
    hspi1.Instance =
        SPI1;

    hspi1.Init.Mode =
        SPI_MODE_MASTER;

    hspi1.Init.Direction =
        SPI_DIRECTION_2LINES;

    hspi1.Init.DataSize =
        SPI_DATASIZE_8BIT;

    hspi1.Init.CLKPolarity =
        SPI_POLARITY_LOW;

    hspi1.Init.CLKPhase =
        SPI_PHASE_1EDGE;

    hspi1.Init.NSS =
        SPI_NSS_SOFT;

    hspi1.Init.BaudRatePrescaler =
        SPI_BAUDRATEPRESCALER_4;

    hspi1.Init.FirstBit =
        SPI_FIRSTBIT_MSB;

    hspi1.Init.TIMode =
        SPI_TIMODE_DISABLE;

    hspi1.Init.CRCCalculation =
        SPI_CRCCALCULATION_DISABLE;

    hspi1.Init.CRCPolynomial =
        10;


    if (
        HAL_SPI_Init(
            &hspi1
        ) != HAL_OK
    )
    {
        Error_Handler();
    }
}


/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig =
        {0};

    TIM_OC_InitTypeDef sConfigOC =
        {0};


    htim2.Instance =
        TIM2;

    htim2.Init.Prescaler =
        0;

    htim2.Init.CounterMode =
        TIM_COUNTERMODE_UP;

    htim2.Init.Period =
        65535;

    htim2.Init.ClockDivision =
        TIM_CLOCKDIVISION_DIV1;

    htim2.Init.AutoReloadPreload =
        TIM_AUTORELOAD_PRELOAD_DISABLE;


    if (
        HAL_TIM_PWM_Init(
            &htim2
        ) != HAL_OK
    )
    {
        Error_Handler();
    }


    sMasterConfig.MasterOutputTrigger =
        TIM_TRGO_RESET;

    sMasterConfig.MasterSlaveMode =
        TIM_MASTERSLAVEMODE_DISABLE;


    if (
        HAL_TIMEx_MasterConfigSynchronization(
            &htim2,
            &sMasterConfig
        ) != HAL_OK
    )
    {
        Error_Handler();
    }


    sConfigOC.OCMode =
        TIM_OCMODE_PWM1;

    sConfigOC.Pulse =
        0;

    sConfigOC.OCPolarity =
        TIM_OCPOLARITY_HIGH;

    sConfigOC.OCFastMode =
        TIM_OCFAST_DISABLE;


    if (
        HAL_TIM_PWM_ConfigChannel(
            &htim2,
            &sConfigOC,
            TIM_CHANNEL_1
        ) != HAL_OK
		)
    {
        Error_Handler();
    }


    HAL_TIM_MspPostInit(
        &htim2
    );
}


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
    huart1.Instance =
        USART1;

    huart1.Init.BaudRate =
        115200;

    huart1.Init.WordLength =
        UART_WORDLENGTH_8B;

    huart1.Init.StopBits =
        UART_STOPBITS_1;

    huart1.Init.Parity =
        UART_PARITY_NONE;

    huart1.Init.Mode =
        UART_MODE_TX_RX;

    huart1.Init.HwFlowCtl =
        UART_HWCONTROL_NONE;

    huart1.Init.OverSampling =
        UART_OVERSAMPLING_16;


    if (
        HAL_UART_Init(
            &huart1
        ) != HAL_OK
    )
    {
        Error_Handler();
    }
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct =
        {0};


    /*
     * GPIO clocks
     */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    /*
     * Initial output states
     */

    /*
     * PC13
     */
    HAL_GPIO_WritePin(
        GPIOC,
        GPIO_PIN_13,
        GPIO_PIN_RESET
    );


    /*
     * PA8 RF
     */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_8,
        GPIO_PIN_RESET
    );


    /*
     * W5500 CS + RESET
     */
    HAL_GPIO_WritePin(
        GPIOB,
        CS_Pin |
        RESET_Pin,
        GPIO_PIN_SET
    );


    /*
     * PC13 LED
     */
    GPIO_InitStruct.Pin =
        GPIO_PIN_13;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;


    HAL_GPIO_Init(
        GPIOC,
        &GPIO_InitStruct
    );


    /*
     * PA8 RF DATA
     */
    GPIO_InitStruct.Pin =
        GPIO_PIN_8;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_HIGH;


    HAL_GPIO_Init(
        GPIOA,
        &GPIO_InitStruct
    );


    /*
     * W5500 CS + RESET
     */
    GPIO_InitStruct.Pin =
        CS_Pin |
        RESET_Pin;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_HIGH;


    HAL_GPIO_Init(
        GPIOB,
        &GPIO_InitStruct
    );
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line number
  * @retval None
  */
void assert_failed(
    uint8_t *file,
    uint32_t line
)
{
    printf(
        "Wrong parameters value: file %s on line %lu\r\n",
        file,
        line
    );
}

#endif /* USE_FULL_ASSERT */
