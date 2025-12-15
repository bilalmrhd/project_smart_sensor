#ifndef _FUNCTIONS_AND_MAIN_RELATED_H_
#define _FUNCTIONS_AND_MAIN_RELATED_H_

// Debug and inference mode control
#define PRINTF_debug 1U   // Enable/disable debug (PRINTF) mode
#define INFERENCE_MODE 1U // Switch between acquisition and inference mode
int b_USE_NPU = 1; // Switch between running the model on the NPU or MCU 

// To change the serial baud rate refer to BOARD_DEBUG_UART_BAUDRATE in the header 'board.h'

/*******************************************************************************
* Include necessary hardware and software headers
******************************************************************************/

#include "fsl_device_registers.h"

#if defined(FSL_FEATURE_SOC_PORT_COUNT) && (FSL_FEATURE_SOC_PORT_COUNT)
#include "fsl_port.h"
#endif

#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "fsl_gpio.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "model/custom_model.h" // Include custom AI model
#include "model/custom_output_postproc.h" // Include custom output post-processing for the AI model
#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// To change the serial baud rate, refer to BOARD_DEBUG_UART_BAUDRATE in the header 'board.h'

/*******************************************************************************
* Definitions
******************************************************************************/

/* Master related definitions for SPI communication */
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI1) // Base address for SPI

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs0) // Chip Select (PCS) used for initialization
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0) // Chip Select (PCS) used for data transfer

#define LPSPI_MASTER_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(1u) // SPI clock frequency
#define TRANSFER_SIZE 3U           /* Data size to transfer via SPI */
#define RECEIVE_SIZE 8U            /* Data size to receive via SPI */
#define TRANSFER_BAUDRATE 2000000U /* Baud rate for SPI communication - set to 2 MHz */

/* Accelerometer related */

/* Registers address (refer to the accelerometer datasheet) */
#define OUT_X_LSB 0x04 // X-axis lower byte
#define OUT_X_MSB 0x05 // X-axis upper byte
#define OUT_Y_LSB 0x06 // Y-axis lower byte
#define OUT_Y_MSB 0x07 // Y-axis upper byte
#define OUT_Z_LSB 0x08 // Z-axis lower byte
#define OUT_Z_MSB 0x09 // Z-axis upper byte

#define WHO_I_AM 0x13 // Accelerometer identification register

#define INT_EN 0x20 // Interrupt enable register
#define INT_PIN_SEL 0x21 // Interrupt pin selection register

#define SENS_CONFIG1 0x15 // Sensor configuration 1
#define SENS_CONFIG2 0x16 // Sensor configuration 2
#define SENS_CONFIG3 0x17 // Sensor configuration 3
#define SENS_CONFIG4 0x18 // Sensor configuration 4
#define SENS_CONFIG5 0x19 // Sensor configuration 5

/* Values assigned to different registers */
#define SENS_CONFIG1_value 0b00000011 // Set accelerometer to 4g range and active mode
#define SENS_CONFIG2_value 0b01010000 // High-performance mode for wake/sleep
#define SENS_CONFIG3_value 0b00100010 // Output data rate (ODR) of 800 Hz
#define SENS_CONFIG4_value 0b00001001
#define SENS_CONFIG5_value 0b00000010 // Skip Z-axis on auto-increment mode

#define INT_EN_value 0b10000000      // Enable data ready interrupt
#define INT_PIN_SEL_value 0b00000000 // Interrupt enabled on INT1 pin

/* Accelerometer sensitivity for different ranges */
#define ACCEL_2G_SENS (0.000976)   /* Sensitivity factor for 2G range */
#define ACCEL_4G_SENS (0.001953)   /* Sensitivity factor for 4G range */
#define ACCEL_8G_SENS (0.003906)   /* Sensitivity factor for 8G range */
#define ACCEL_16G_SENS (0.007813)  /* Sensitivity factor for 16G range */
#define CURRENT_SENS ACCEL_4G_SENS // Default sensitivity is set to 4G range

#define LIST_SIZE 3286 // Number of records stored per sequence

// Structure to store raw accelerometer data for one record
typedef struct
{
    int16_t accelraw[RECEIVE_SIZE / 2 - 1]; // Stores raw acceleration data
} fxls8974_data;

// Dynamic list structure (not used in the current code)
typedef struct
{
    int16_t *array; // Pointer to dynamic array
    size_t used;    // Number of elements currently used
    size_t size;    // Maximum size of the list
} DynamicList;

// Static list structure used in the code

typedef struct
{
    int16_t array[LIST_SIZE]; // Fixed-size array to store data
    int used; // Number of used elements in the list
} StaticList;

// Structure to store full accelerometer data (X and Y axis) before sending or processing
typedef struct
{
    StaticList X_accel; // List of X-axis acceleration values
    StaticList Y_accel; // List of Y-axis acceleration values
} fxls8974_full_data;

/* Board related pin configurations */

#define BOARD_INT1_PORT PORT0
#define BOARD_INT1_GPIO GPIO0 // INT1 pin for accelerometer is defined with both PORT0 and GPIO0
#define BOARD_INT1_PIN 10U    // Pin P0_10 for accelerometer's INT1 pin

#define BOARD_BUTTON_PORT PORT0
#define BOARD_BUTTON_GPIO GPIO0 // Button pin is defined with both PORT0 and GPIO0
#define BOARD_BUTTON_PIN 19U    // Pin P0_28 for the button

#define BOARD_LED_GPIO GPIO5
#define BOARD_LED_PORT PORT5 // LED pin is defined with both PORT5 and GPIO5
#define BOARD_RED_GPIO_PIN 4U   // Pin P5_4 for red LED on the RGB LED
#define BOARD_GREEN_GPIO_PIN 3U // Pin P5_3 for green LED on the RGB LED
#define BOARD_BLUE_PIN 2U  // Pin P5_2 for blue LED on the RGB LED

/* State machine related definitions */
typedef enum
{
    INIT,    // Initialization state
    ACQUIRE, // Data acquisition state
    SAVE,    // Data saving state
    SEND,    // Data sending state
    INFER,   // Inference state (AI model)
} State;

typedef struct
{
    State current_state; // Current state of the state machine
} StateMachine;

/*******************************************************************************
* Variables
******************************************************************************/

uint8_t masterRxData[RECEIVE_SIZE] = {0U}; // SPI received data buffer
uint8_t masterTxData[RECEIVE_SIZE] = {0U}; // SPI transmit data buffer

uint32_t srcClock_Hz;
lpspi_master_config_t masterConfig; // SPI master configuration
lpspi_transfer_t masterXfer;        // SPI transfer structure

volatile bool isTransferCompleted = false; // Flag indicating if SPI transfer is completed
lpspi_master_handle_t g_m_handle;          // Handle for the SPI transfer callback

bool INT1_state;   // Stores the state of the INT1 pin
bool button_state; // Stores the state of the button pin

StateMachine accel_machine;      // State machine for accelerometer operation
fxls8974_data temporary_data;    // Temporary storage for one accelerometer record
fxls8974_full_data data_to_send; // Full storage for accelerometer data to send or infer

/* Model related */
tensor_dims_t inputDims; // Input tensor dimensions for AI model
tensor_type_t inputType; // Input tensor data type for AI model
int8_t *inputData;       // Pointer to input tensor data
tensor_dims_t outputDims; // Output tensor dimensions for AI model
tensor_type_t outputType; // Output tensor data type for AI model
int8_t *outputData;       // Pointer to output tensor data

/*******************************************************************************
* Functions
******************************************************************************/

/*!
* @brief Initializes GPIO pins
*/
void InitGPIO();

/*!
* @brief Creates a delay
* @param delay_ms Desired delay in milliseconds
*/
void timer_delay(uint32_t delay_ms);

/*!
* @brief Initializes the FXLS8974CF Accelerometer
*/
void init_accel();

/*!
* @brief Acquires data from the FXLS8974CF Accelerometer
*/
void acquire_data();

/*!
* @brief Converts raw accelerometer data from two's complement to decimal
*/
int16_t twos_complement_to_decimal(uint16_t binary, uint8_t num_bits);

/*!
* @brief Handles data acquisition mode process
* @param machine Pointer to the state machine structure
*/
void data_acquisition_mode(StateMachine *machine);
/*!

* @brief Handles the initialization state of the state machine
* @param machine Pointer to the state machine structure
*/
void init_state(StateMachine *machine);

/*!
* @brief Handles the data acquisition state of the state machine
* @param machine Pointer to the state machine structure
*/
void acquire_state(StateMachine *machine);

/*!
* @brief Handles the data saving state of the state machine
* @param machine Pointer to the state machine structure
*/
void save_state(StateMachine *machine);

/*!
* @brief Handles the data sending state via UART in the state machine
* @param machine Pointer to the state machine structure
*/
void send_state(StateMachine *machine);

/*!
* @brief Handles the AI model inference process
* @param machine Pointer to the state machine structure
*/
void inference_mode(StateMachine *machine);

/*!
* @brief Handles the inference state of the state machine
* @param machine Pointer to the state machine structure
*/
void infer_state(StateMachine *machine);

/*!
* @brief Initializes a static list (used for accelerometer data)
* @param list Pointer to the static list structure
*/
void initList(StaticList *list);

/*!
* @brief Adds an element to the static list
* @param list Pointer to the static list structure
* @param element Element to add to the list
*/
void addElement(StaticList *list, int16_t element);

/*!
* @brief Empty the static list
* @param list Pointer to the static list structure
*/
void freeList(StaticList *list);

#endif // _FUNCTIONS_AND_MAIN_RELATED_H_
