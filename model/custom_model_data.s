    .section .rodata
    .align 16
    .global custom_model_data_npu
    .global custom_model_data_npu_end
    .global custom_model_data_mcu
    .global custom_model_data_mcu_end

custom_model_data_npu: 
    .incbin "model/tf_model_int8_v12_converted_cropped.tflite"
custom_model_data_npu_end:

custom_model_data_mcu: 
    .incbin "model/tf_model_int8_v12_cropped.tflite"
custom_model_data_mcu_end:

