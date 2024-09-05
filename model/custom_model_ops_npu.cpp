/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/kernels/neutron/neutron.h"

extern int v_USE_NPU;
extern "C" const char custom_model_data_npu[], custom_model_data_npu_end[], custom_model_data_mcu[], custom_model_data_mcu_end[];

tflite::MicroOpResolver &MODEL_GetOpsResolver()
{
    if(v_USE_NPU)   MicroPrintf("using %s , model data 0x%x, len=0x%x \n", "NPU", custom_model_data_npu, custom_model_data_npu_end-custom_model_data_npu);
    else            MicroPrintf("using %s , model data 0x%x, len=0x%x \n", "MCU", custom_model_data_mcu, custom_model_data_mcu_end-custom_model_data_mcu);

    static tflite::MicroMutableOpResolver<5> s_microOpResolver;
if(v_USE_NPU) {
    // For NPU model
    s_microOpResolver.AddMean(); // Faut trouver le model avec une couche mean
    s_microOpResolver.AddCustom(tflite::GetString_NEUTRON_GRAPH(), tflite::Register_NEUTRON_GRAPH());
} else {
    // For MCU model
    s_microOpResolver.AddExpandDims();
    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddReshape();
    s_microOpResolver.AddMean();
    s_microOpResolver.AddFullyConnected();
}
    return s_microOpResolver;
}
