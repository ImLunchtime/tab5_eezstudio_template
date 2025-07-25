if(ESP_PLATFORM)
    # for ESP-IDF platform
    get_filename_component(dir ${CMAKE_CURRENT_LIST_FILE} PATH)
    FILE(GLOB_RECURSE app_sources ${dir}/src/eez/*.c*)
    idf_component_register(SRCS ${app_sources}      
        INCLUDE_DIRS "src" "src/eez/libs/agg"
        PRIV_REQUIRES lvgl
    )
    
    # Ensure EEZ_FOR_LVGL and LV_LVGL_H_INCLUDE_SIMPLE are defined for this component
    target_compile_definitions(${COMPONENT_LIB} PUBLIC 
        EEZ_FOR_LVGL=1
        LV_LVGL_H_INCLUDE_SIMPLE=1
    )
    
    # Disable dangling-pointer warning for this component
    target_compile_options(${COMPONENT_LIB} PRIVATE 
        -Wno-dangling-pointer
        -Wno-error=dangling-pointer
    )
else()
    cmake_minimum_required(VERSION 3.12)

    include_directories(
        ./src
        ./src/eez/libs/agg
    )

    file(GLOB_RECURSE SOURCES
        ./src/eez/*.cpp
        ./src/eez/*.c
    )

    ADD_LIBRARY(eez-framework STATIC ${SOURCES})

    target_include_directories(eez-framework SYSTEM PUBLIC ./src ./src/eez/libs/agg)
endif()
