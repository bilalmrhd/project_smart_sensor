/*
* Copyright 2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _CUSTOM_MODEL_H_
#define _CUSTOM_MODEL_H_

#include <stdint.h>

#include "fsl_common.h"

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

/*******************************************************************************
* Definitions
******************************************************************************/

  // The name of the AI model being used
#define MODEL_NAME "Symbol_recognition"

// Modify these values to match the input/output scales and tensor configurations for your specific model
#define MAX_TENSOR_DIMS 3 // Maximum number of dimensions in the input/output tensors

// These values are related to quantization in the AI model, adjusting the input and output scales
#define MODEL_SCALE_INPUT  8.960783f//8.662745475f   // Input scaling factor (converts input values to model-specific scale)
#define MODEL_SCALE_OUTPUT 0.1375229f//0.163708925f   // Output scaling factor (converts output values from model-specific scale)
#define MODEL_INPUT_ZERO_POINT -15//-20       // Zero-point for input data (used in quantized models)
#define MODEL_OUTPUT_ZERO_POINT -10//-128       // Zero-point for output data (used in quantized models)

// Structure to represent the dimensions of the input and output tensors
typedef struct
{
  uint32_t size;              // Number of dimensions in the tensor (e.g., 2D, 3D, etc.)
  uint32_t data[MAX_TENSOR_DIMS]; // Array holding the size of each dimension (e.g., [batch_size, height, width])
} tensor_dims_t;

// Enum to represent the data type of the tensors (e.g., float, int8, uint8)
typedef enum
{
  kTensorType_FLOAT32 = 0, // Floating-point data type (32-bit)
  kTensorType_UINT8 = 1,   // Unsigned 8-bit integer data type
  kTensorType_INT8 = 2     // Signed 8-bit integer data type
} tensor_type_t;

  // Function prototypes for interacting with the model

  /*!
   * @brief Get the amount of memory used by the model's arena (dynamic memory allocation).
   * @param pMaxSize Pointer to store the maximum size of memory used.
   * @return Size of memory used in bytes.
   */
size_t MODEL_GetArenaUsedBytes(size_t *pMaxSize);

  /*!
   * @brief Initialize the model (optionally using the NPU or MCU).
   * @param useNPU Flag to indicate whether to use the NPU (1) or the MCU (0).
   * @return Status of initialization (kStatus_Success if successful).
   */
status_t MODEL_Init(int useNPU);

  /*!
   * @brief Get a pointer to the input tensor's data for the model.
   * @param dims Pointer to a tensor_dims_t struct that will be populated with the input tensor's dimensions.
   * @param type Pointer to a tensor_type_t enum that will be populated with the input tensor's data type.
   * @return Pointer to the input tensor data (int8_t*).
   */
int8_t *MODEL_GetInputTensorData(tensor_dims_t *dims, tensor_type_t *type);

  /*!
   * @brief Get a pointer to the output tensor's data for the model.
   * @param dims Pointer to a tensor_dims_t struct that will be populated with the output tensor's dimensions.
   * @param type Pointer to a tensor_type_t enum that will be populated with the output tensor's data type.
   * @return Pointer to the output tensor data (int8_t*).
   */
int8_t *MODEL_GetOutputTensorData(tensor_dims_t *dims, tensor_type_t *type);

  /*!
   * @brief Runs inference using the loaded model.
   * @return Status of the inference operation (kStatus_Success if successful).
   */
status_t MODEL_RunInference(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _CUSTOM_MODEL_H_ */