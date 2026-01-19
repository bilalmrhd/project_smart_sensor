    .section .rodata
    .align 16
    .global custom_model_data_npu
    .global custom_model_data_npu_end
    .global custom_model_data_mcu
    .global custom_model_data_mcu_end

custom_model_data_npu: 
    .incbin "model/tflite_quant_model_converted.tflite"
    //.incbin "tf_model_int8_v12_cropped_converted.tflite"
custom_model_data_npu_end:

custom_model_data_mcu: 
    .incbin "model/tflite_quant_model_converted.tflite"
    //.incbin "model/tf_model_int8_v12_cropped.tflite"
custom_model_data_mcu_end:

