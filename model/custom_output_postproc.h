/*
* Copyright 2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _CUSTOM_OUTPUT_POSTPROCESS_H_
#define _CUSTOM_OUTPUT_POSTPROCESS_H_

#include "custom_model.h" // Include the model header
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus*/

/*!
* @brief Structure to store the result of inference, including the score and the index of the predicted class.
*/
typedef struct
{
    float score;  // The probability score of the predicted class
    int index;    // The index of the predicted class
} result_t;

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
* @brief Processes the output data from the AI model inference.
* @param data Pointer to the raw output data from the model.
* @param dims Pointer to the structure holding the dimensions of the output tensor.
* @param type Enum representing the data type of the output tensor (e.g., INT8, FLOAT32).
* @param inferenceTime The time taken to run the inference in microseconds.
* @return Status of the post-processing operation (kStatus_Success if successful).
*/
status_t MODEL_ProcessOutput(const int8_t *data, const tensor_dims_t *dims,
                            tensor_type_t type, int inferenceTime);

/*!
* @brief Computes the softmax of the output data, turning logits into probabilities.
* @param data Array of logits (output values before applying softmax).
* @param dims Pointer to the structure holding the dimensions of the output tensor.
* @return Pointer to the array of probabilities.
*/
float *softmax(const float data[], const tensor_dims_t *dims);

/*!
* @brief Finds the class with the highest probability from the softmax output.
* @param probabilities Array of probabilities obtained from the softmax function.
* @param dims Pointer to the structure holding the dimensions of the output tensor.
* @return A structure containing the highest probability score and its corresponding class index.
*/
result_t find_max(float *probabilities, const tensor_dims_t *dims);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CUSTOM_OUTPUT_POSTPROCESS_H_ */