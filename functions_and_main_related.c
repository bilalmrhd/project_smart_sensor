#include "functions_and_main_related.h"

/*******************************************************************************
 * Functions Code
 ******************************************************************************/

void InitGPIO()
{
    /* Define configuration structures for different GPIO pins */
    gpio_pin_config_t config_INT1_Accel =
        {
            kGPIO_DigitalInput, // Set accelerometer INT1 pin as digital input
            0,                  // No initial output level
        };

    gpio_pin_config_t config_button =
        {
            kGPIO_DigitalInput, // Set button pin as digital input
            0,
        };

    gpio_pin_config_t config_led_red =
        {
            kGPIO_DigitalOutput, // Set red LED as digital output
            0,
        };

    gpio_pin_config_t config_led_green =
        {
            kGPIO_DigitalOutput, // Set green LED as digital output
            0,
        };

    gpio_pin_config_t config_led_blue =
        {
            kGPIO_DigitalOutput, // Set blue LED as digital output
            0,
        };

    CLOCK_EnableClock(kCLOCK_Port0); // Enable the clock for PORT0 (GPIO)

    /* Configure pin multiplexing for the button, accelerometer, and LEDs */
    PORT_SetPinMux(BOARD_BUTTON_PORT, BOARD_BUTTON_PIN, kPORT_MuxAlt0);
    PORT_SetPinMux(BOARD_INT1_PORT, BOARD_INT1_PIN, kPORT_MuxAlt0);
    PORT_SetPinMux(BOARD_LED_PORT, BOARD_BLUE_PIN, kPORT_MuxAlt0);

    /* Configure button pin with internal pull-up resistor */
    BOARD_BUTTON_PORT->PCR[BOARD_BUTTON_PIN] = ((BOARD_BUTTON_PORT->PCR[BOARD_BUTTON_PIN] &
                                                (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IBE_MASK))) |
                                                PORT_PCR_PS(1) | PORT_PCR_PE(1) | PORT_PCR_IBE(1));

    /* Configure accelerometer INT1 pin with internal pull-up resistor */
    BOARD_INT1_PORT->PCR[BOARD_INT1_PIN] = ((BOARD_INT1_PORT->PCR[BOARD_INT1_PIN] &
                                            (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IBE_MASK))) |
                                            PORT_PCR_PS(1) | PORT_PCR_PE(1) | PORT_PCR_IBE(1));

    /* Initialize GPIO pins with their respective configurations */
    GPIO_PinInit(BOARD_BUTTON_GPIO, BOARD_BUTTON_PIN, &config_button);
    GPIO_PinInit(BOARD_INT1_GPIO, BOARD_INT1_PIN, &config_INT1_Accel);
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_BLUE_PIN, &config_led_blue);
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_GREEN_GPIO_PIN, &config_led_green);
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_RED_GPIO_PIN, &config_led_red);

    /* Enable GPIO input pins for the button and accelerometer INT1 */
    GPIO_PortInputEnable(BOARD_BUTTON_GPIO, 1 << BOARD_BUTTON_PIN);
    GPIO_PortInputEnable(BOARD_INT1_GPIO, 1 << BOARD_INT1_PIN);
}
void timer_delay(uint32_t value)
{
    uint32_t delay_us = value * 1000;
    clock_t start_time = TIMER_GetTimeInUS();           // Get the start time in microseconds
    while (TIMER_GetTimeInUS() < start_time + delay_us); // Loop until the delay time is reached
}

void init_accel()
{
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Start initialization sequence...\r\n");
#endif

    /* Set accelerometer to inactive mode */
    masterTxData[0] = SENS_CONFIG1;
    masterTxData[1] = 0;
    masterTxData[2] = 0;

    isTransferCompleted = false;                                                                                         // Reset the transfer completion flag
    masterXfer.txData = masterTxData;                                                                                    // Set the data to be sent
    masterXfer.rxData = NULL;                                                                                            // No data to receive
    masterXfer.dataSize = 3U;                                                                                            // Set data size
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap; // SPI transfer configuration
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);                            // Start non-blocking SPI transfer

    while (!isTransferCompleted)
    {
    }                // Wait for the transfer to complete
    timer_delay(10); // Delay to allow hardware to process

    masterTxData[0] = SENS_CONFIG2;
    masterTxData[1] = 0;
    masterTxData[2] = SENS_CONFIG2_value;
    isTransferCompleted = false;
    masterXfer.txData = masterTxData;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 3U;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);
    while (!isTransferCompleted)
    {
    }
    timer_delay(10);

    masterTxData[0] = SENS_CONFIG3;
    masterTxData[1] = 0;
    masterTxData[2] = SENS_CONFIG3_value;
    isTransferCompleted = false;
    masterXfer.txData = masterTxData;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 3U;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);
    while (!isTransferCompleted)
    {
    }
    timer_delay(10);

    masterTxData[0] = SENS_CONFIG4;
    masterTxData[1] = 0;
    masterTxData[2] = SENS_CONFIG4_value;
    isTransferCompleted = false;
    masterXfer.txData = masterTxData;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 3U;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);
    while (!isTransferCompleted)
    {
    }
    timer_delay(10);

    // Uncomment if you want to only acquire X and Y acceleration
    masterTxData[0] = SENS_CONFIG5;
    masterTxData[1] = 0;
    masterTxData[2] = SENS_CONFIG5_value;
    isTransferCompleted = false;
    masterXfer.txData = masterTxData;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 3U;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);
    while (!isTransferCompleted)
    {
    }
    timer_delay(10);

    masterTxData[0] = INT_EN;
    masterTxData[1] = 0;
    masterTxData[2] = INT_EN_value;
    isTransferCompleted = false;
    masterXfer.txData = masterTxData;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 3U;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);
    while (!isTransferCompleted)
    {
    }
    timer_delay(10);

    masterTxData[0] = INT_PIN_SEL;
    masterTxData[1] = 0;
    masterTxData[2] = INT_PIN_SEL_value;
    isTransferCompleted = false;
    masterXfer.txData = masterTxData;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 3U;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);
    while (!isTransferCompleted)
    {
    }
    timer_delay(10);

    /* Last thing to do is to set the accel in active mode */
    masterTxData[0] = SENS_CONFIG1;
    masterTxData[1] = 0;
    masterTxData[2] = SENS_CONFIG1_value;
    isTransferCompleted = false;
    masterXfer.txData = masterTxData;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = 3U;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);
    while (!isTransferCompleted)
    {
    }
    timer_delay(10);

#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Initialiazation completed\r\n");
    PRINTF("\r\n");
#endif
}

void acquire_data()
{
    masterTxData[0] = OUT_X_LSB | (1 << 7); // Set the address to read data from X-axis (Auto-increment mode is activated)
    masterTxData[1] = 0;
    masterTxData[2] = 0;

    isTransferCompleted = false;                                                                                         // Reset the transfer completion flag
    masterXfer.txData = masterTxData;                                                                                    // Set the data to be sent
    masterXfer.rxData = masterRxData;                                                                                    // Set the buffer to store received data
    masterXfer.dataSize = RECEIVE_SIZE;                                                                                  // Set data size to receive
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap; // SPI transfer configuration
    LPSPI_MasterTransferNonBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);                            // Start non-blocking SPI transfer

    while (!isTransferCompleted)
    {
    } // Wait for the transfer to complete
}

int16_t twos_complement_to_decimal(uint16_t binary, uint8_t num_bits)
{
    int16_t mask = 1 << (num_bits - 1);
    if (binary & mask) // If the sign bit is set, it's a negative number
    {
        uint16_t inverted = ~binary + 1; // Invert and add 1 to get the negative number
        return -(int16_t)(inverted & ((1 << num_bits) - 1));
    }
    else // Positive number
    {
        return (int16_t)binary;
    }
}

void data_acquisition_mode(StateMachine *machine)
{
    switch (machine->current_state)
    {
    case INIT:
        init_state(machine); // Initialize the system
        break;
    case ACQUIRE:
        acquire_state(machine); // Acquire data
        break;
    case SAVE:
        save_state(machine); // Save data
        break;
    case SEND:
        send_state(machine); // Send data
        break;
    default:
        init_state(machine); // Default to initialization
        break;
    }
}

void init_state(StateMachine *machine)
{
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Init state\r\n");
#endif
    /* Indicate state with LEDs */
    GPIO_PortClear(BOARD_LED_GPIO, 1 << BOARD_GREEN_GPIO_PIN); // Turn off green LED
    GPIO_PortSet(BOARD_LED_GPIO, 1 << BOARD_RED_GPIO_PIN);     // Turn on red LED

    initList(&data_to_send.X_accel); // Initialize list for X-axis data
    initList(&data_to_send.Y_accel); // Initialize list for Y-axis data

    /* Check the button state to avoid bouncing issues */
    button_state = GPIO_PinRead(BOARD_BUTTON_GPIO, BOARD_BUTTON_PIN);
    if (button_state) // If button is pressed
    {
        do
        {
            button_state = GPIO_PinRead(BOARD_BUTTON_GPIO, BOARD_BUTTON_PIN);
        } while (button_state); // Wait until button is released
        timer_delay(1);                                          // Small delay
        GPIO_PortClear(BOARD_LED_GPIO, 1 << BOARD_RED_GPIO_PIN); // Turn off red LED
        GPIO_PortSet(BOARD_LED_GPIO, 1 << BOARD_GREEN_GPIO_PIN); // Turn on green LED
        machine->current_state = ACQUIRE;                        // Move to acquire state
    }
    else
        machine->current_state = INIT; // Stay in init state if button not pressed
}

void acquire_state(StateMachine *machine)
{
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Acquire state\r\n");
#endif
    /* Acquire raw data from accelerometer */
    acquire_data();

    /* Convert the raw data to decimal and store it */
    temporary_data.accelraw[0] = twos_complement_to_decimal((masterRxData[3] << 8) | masterRxData[2], 16); // X-axis
    temporary_data.accelraw[1] = twos_complement_to_decimal((masterRxData[5] << 8) | masterRxData[4], 16); // Y-axis

    /* Add the converted data to the sequence */
    addElement(&data_to_send.X_accel, temporary_data.accelraw[0]);
    addElement(&data_to_send.Y_accel, temporary_data.accelraw[1]);

    machine->current_state = SAVE; // Move to save state
}

void save_state(StateMachine *machine)
{
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Save state\r\n");
#endif
    /* Check if the maximum number of records (LIST_SIZE) is reached */
    if (data_to_send.X_accel.used >= LIST_SIZE)
    {
        GPIO_PortSet(BOARD_LED_GPIO, 1 << BOARD_RED_GPIO_PIN);     // Turn on red LED
        GPIO_PortClear(BOARD_LED_GPIO, 1 << BOARD_GREEN_GPIO_PIN); // Turn off green LED
        machine->current_state = SEND;                             // Move to send state
        PRINTF("Too long sequence");
    }
    else
    {
        /* Check if the button is pressed to end the acquisition */
        button_state = GPIO_PinRead(BOARD_BUTTON_GPIO, BOARD_BUTTON_PIN);
        if (button_state) // Button pressed
        {
            do
            {
                button_state = GPIO_PinRead(BOARD_BUTTON_GPIO, BOARD_BUTTON_PIN);
            } while (button_state); // Wait until the button is released

#if defined(INFERENCE_MODE) && INFERENCE_MODE
            /* In inference mode, go to inference state */
            GPIO_PortSet(BOARD_LED_GPIO, 1 << BOARD_RED_GPIO_PIN);     // Turn on red LED
            GPIO_PortClear(BOARD_LED_GPIO, 1 << BOARD_GREEN_GPIO_PIN); // Turn off green LED
            machine->current_state = INFER;
#else
            machine->current_state = SEND; // Otherwise, go to send state
#endif
        }
        else
        {
            /* Read INT1 pin to check if more data is available */
            INT1_state = GPIO_PinRead(BOARD_INT1_GPIO, BOARD_INT1_PIN);
#if defined(PRINTF_debug) && PRINTF_debug
            PRINTF("INT1 state: %d\r\n", INT1_state);
#endif
            if (!INT1_state)                   // If INT1 pin is low (data ready)
                machine->current_state = SAVE; // Stay in save state
            else
            {
                do
                {
                    INT1_state = GPIO_PinRead(BOARD_INT1_GPIO, BOARD_INT1_PIN);
                } while (INT1_state); // Wait until INT1 pin goes high (data not ready)
                machine->current_state = ACQUIRE; // Go back to acquire state
            }
        }
    }
}

void send_state(StateMachine *machine)
{
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Send state\r\n");
#endif

    /* Indicate sending with LEDs */
    GPIO_PortSet(BOARD_LED_GPIO, 1 << BOARD_GREEN_GPIO_PIN); // Turn on green LED
    GPIO_PortSet(BOARD_LED_GPIO, 1 << BOARD_RED_GPIO_PIN);   // Turn on red LED

    /* Print the collected data */
    int i;
    for (i = 0; i < LIST_SIZE; i++)
    {
        PRINTF("x: %d y: %d\n", data_to_send.X_accel.array[i], data_to_send.Y_accel.array[i]);
    }

    /* Clear the static lists */
    freeList(&data_to_send.X_accel);
    freeList(&data_to_send.Y_accel);

#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("End\r\n");
#endif
    machine->current_state = INIT; // Reset the state machine to the initialization state
}

void inference_mode(StateMachine *machine)
{
    switch (machine->current_state)
    {
    case INIT:
        init_state(machine);
        break;
    case ACQUIRE:
        acquire_state(machine);
        break;
    case SAVE:
        save_state(machine);
        break;
    case INFER:
        infer_state(machine); 
        break;
    default:
        init_state(machine);
        break;
    }
}

#if defined(INFERENCE_MODE) && INFERENCE_MODE
void infer_state(StateMachine *machine)
{
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Infer state\r\n");
#endif
    int size = inputDims.data[2] * inputDims.data[1]; // Get the size of the input tensor
    int i, k = 0;
    inputType = kTensorType_INT8; // Set the input type as int8
    for (i = 0; i < size; i += 2) // Loop through the input data
    {
        if (i >= data_to_send.X_accel.used) // If we've used all the data
        {
            inputData[i] = MODEL_INPUT_ZERO_POINT; // Fill with zero point values
            inputData[i + 1] = MODEL_INPUT_ZERO_POINT;
        }
        else // Otherwise, quantize the data to fit the model's input requirements
        {
            inputData[i] = (int8_t)(((float)(data_to_send.X_accel.array[k] * CURRENT_SENS)) / MODEL_SCALE_INPUT) + MODEL_INPUT_ZERO_POINT;
            inputData[i + 1] = (int8_t)(((float)(data_to_send.Y_accel.array[k] * CURRENT_SENS)) / MODEL_SCALE_INPUT) + MODEL_INPUT_ZERO_POINT;
            k++;
        }
    }

#if defined(PRINTF_debug) && PRINTF_debug
    /* Print input data for debugging */
    PRINTF("Input value:\n");
    for (i = 0; i < data_to_send.X_accel.used; i += 2)
    {
        PRINTF("i: %d x: %d y: %d\n", i, inputData[i], inputData[i + 1]);
    }
    PRINTF("Input Tensor set  \r\n");
#endif

    /* Run the AI model inference */
    int startTime = TIMER_GetTimeInUS();
    MODEL_RunInference();
    int endTime = TIMER_GetTimeInUS();

#if defined(PRINTF_debug) && PRINTF_debug
    /* Print output data for debugging */
    PRINTF("Output value:\n");
    for (i = 0; i < outputDims.data[1]; i++)
    {
        PRINTF("class: %d prob: %d\n", i, outputData[i]);
    }
#endif

    /* Process the model output */
    MODEL_ProcessOutput(outputData, &outputDims, outputType, endTime - startTime);

    /* Free the lists and reset the state machine */
    freeList(&data_to_send.X_accel);
    freeList(&data_to_send.Y_accel);
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("End\r\n");
#endif
    machine->current_state = INIT;
}
#endif

void initList(StaticList *sList)
{
    for (int i = 0; i < LIST_SIZE; i++)
    {
        sList->array[i] = 0; // Initialize all elements to 0
    }
    sList->used = 0; // Set the used count to 0
}

void addElement(StaticList *sList, int16_t value)
{
    if (sList->used < LIST_SIZE) // Check if there's space in the list
    {
        sList->array[sList->used] = value; // Add the element
        sList->used++;                     // Increment the used count
    }
    else
    {
        printf("List is full! Cannot add more elements.\n");
    }
}

void freeList(StaticList *sList)
{
    for (int i = 0; i < LIST_SIZE; i++)
    {
        sList->array[i] = 0; // Reset all elements to 0
    }
    sList->used = 0; // Reset the used count
}
