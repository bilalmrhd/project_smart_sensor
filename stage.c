#include "functions_and_main_related.h"

/*******************************************************************************
 * Functions
 ******************************************************************************/

/*!
 * @brief This is the LPSPI user callback function. This function is called when the SPI transfer is completed
 */
void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP(); // No Operation, used as a placeholder
    }
    isTransferCompleted = true; // Mark the transfer as completed
}
/*******************************************************************************
 * Main Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Attach FRO 12M (Free Running Oscillator) to FLEXCOMM4 (for the debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* Attach FRO 12M to FLEXCOMM1 (another communication interface) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom1Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);

    /* Enable the clock for GPIO (General Purpose Input/Output) */
    CLOCK_EnableClock(kCLOCK_Gpio0);

    /* Initialize board-specific pins, clocks, and debug console */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Initialize the system timer */
    TIMER_Init();

    /* Master SPI configuration */
    LPSPI_MasterGetDefaultConfig(&masterConfig);                                      // Get default SPI configuration
    masterConfig.baudRate = TRANSFER_BAUDRATE;                                        // Set the communication speed (baud rate)
    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;                        // Choose the peripheral chip select (PCS) line
    masterConfig.pcsToSckDelayInNanoSec = 1000000000U / (masterConfig.baudRate * 2U); // Calculate delays based on baud rate
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000U / (masterConfig.baudRate * 2U);
    masterConfig.betweenTransferDelayInNanoSec = 1000000000U / (masterConfig.baudRate * 2U);

    /* Initialize the SPI master with the selected configuration */
    srcClock_Hz = LPSPI_MASTER_CLK_FREQ;
    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    /* Create a handle for the SPI master transfer and assign the callback function */
    LPSPI_MasterTransferCreateHandle(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_handle, LPSPI_MasterUserCallback, NULL);

    /* Initialize GPIO pins */
    InitGPIO();
    timer_delay(10); // Add a delay to allow for hardware setup
    init_accel();    // Initialize accelerometer

    /* Initialize the AI model if inference mode is enabled */
#if defined(INFERENCE_MODE) && INFERENCE_MODE
    if (MODEL_Init(b_USE_NPU) != kStatus_Success) // Initialize the AI model
    {
        PRINTF("Failed initializing model");
        for (;;)
        {
        } // Infinite loop if the model initialization fails
    }
    /* Get pointers to the model's input and output data */
    inputData = MODEL_GetInputTensorData(&inputDims, &inputType);
    outputData = MODEL_GetOutputTensorData(&outputDims, &outputType);
#endif

    /* Debug print to indicate the start of the state machine */
#if defined(PRINTF_debug) && PRINTF_debug
    PRINTF("Starting state machine...\r\n");
#endif

    /* Main loop that repeatedly runs either inference or data acquisition mode */
    while (1)
    {
#if defined(INFERENCE_MODE) && INFERENCE_MODE
        inference_mode(&accel_machine); // Run the AI model inference if enabled
#else
        data_acquisition_mode(&accel_machine); // Otherwise, run data acquisition from the accelerometer
#endif
    }
}
