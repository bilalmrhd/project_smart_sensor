/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

// #include "output_postproc.h"
#include "custom_output_postproc.h"

#include "../custom_labels.h"
#ifdef EIQ_GUI_PRINTF
#include "chgui.h"
#endif

#define EOL "\n\r"

status_t MODEL_ProcessOutput(const int8_t *data, const tensor_dims_t *dims,
                            tensor_type_t type, int inferenceTime)
{
    const char *label = "No label detected";
    float scaled_data[dims->data[1]];

    PRINTF("Output value:\n");
    for (int i = 0; i < dims->data[1]; ++i)
    {
        PRINTF("%d\n", data[i]);
        scaled_data[i] = ((float)(data[i] - MODEL_OUTPUT_ZERO_POINT)) * MODEL_SCALE_OUTPUT;
    }

    float *probabilities = softmax(scaled_data, dims);

    PRINTF("Softmax probabilities:\n");
    for (int i = 0; i < dims->data[1]; ++i)
    {
        PRINTF("%f\n", probabilities[i]);
    }

    result_t confidence = find_max(probabilities, dims);
    label = labels[confidence.index];

    int score = (int)(confidence.score * 100);
    PRINTF("----------------------------------------" EOL);
    PRINTF("     Inference time: %d ms" EOL, inferenceTime / 1000);
    PRINTF("     Detected: %s (%d%%)" EOL, label, score);
    PRINTF("----------------------------------------" EOL);

    free(probabilities);

#ifdef EIQ_GUI_PRINTF
    GUI_PrintfToBuffer(GUI_X_POS, GUI_Y_POS, "Detected: %.20s (%d%%)", label, score);
#endif

    return kStatus_Success;
}

float *softmax(const float data[], const tensor_dims_t *dims)
{
    int length = dims->data[1];
    float *probabilities = (float *)malloc(length * sizeof(float));
    if (probabilities == NULL)
    {
        return NULL;
    }
    float sum_exp = 0.0;
    for (int i = 0; i < length; ++i)
    {
        probabilities[i] = exp(data[i]);
        sum_exp += probabilities[i];
    }
    for (int i = 0; i < length; ++i)
    {
        probabilities[i] /= sum_exp;
    }
    return probabilities;
}

result_t find_max(float *probabilities, const tensor_dims_t *dims)
{
    result_t result;
    int length = dims->data[1];
    float max_value = probabilities[0];
    int max_index = 0;
    for (int i = 1; i < length; ++i)
    {
        if (probabilities[i] > max_value)
        {
            max_value = probabilities[i];
            max_index = i;
        }
    }
    result.score = (float)max_value;
    result.index = max_index;
    return result;
}
