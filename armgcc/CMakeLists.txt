# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
    message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
    return()
endif()


# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(stage)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME stage.elf)

if (NOT DEFINED $SdkRootDirPath)
    SET($SdkRootDirPath ${ProjDirPath}/../../../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../stage.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../timer.h"
"${ProjDirPath}/../timer.c"
"${ProjDirPath}/../functions_and_main_related.h"
"${ProjDirPath}/../functions_and_main_related.c"
"${ProjDirPath}/../model/custom_model.cpp"
"${ProjDirPath}/../model/custom_model_ops_npu.cpp"
"${ProjDirPath}/../model/custom_output_postproc.cpp"
"${ProjDirPath}/../model/custom_model_data.s"
)



target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../model 
)




include(${SdkRootDirPath}/core/devices/MCXN947/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

set(link_directories "${SdkRootDirPath}/middleware/eiq/tensorflow-lite/lib/cm33/armgcc" "${SdkRootDirPath}/middleware/eiq/tensorflow-lite/third_party/neutron/mcxn")
set(link_libraries "tflm" "NeutronDriver" "NeutronFirmware" )


target_link_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE ${link_directories})
TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES} ${link_libraries})
TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")




