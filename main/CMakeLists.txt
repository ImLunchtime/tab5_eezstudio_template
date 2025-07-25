# Automatically scan for UI source files
file(GLOB_RECURSE UI_SOURCES "ui/*.c")

idf_component_register(SRCS "m5stack-tab5-lvgl.c"
                            "hal.c"
                            "gui.c"
                            ${UI_SOURCES}
                    INCLUDE_DIRS "."
                                 "ui"
                    REQUIRES eez-framework lvgl esp_lcd_touch m5stack-tab5)

# Enable EEZ_FOR_LVGL flag
target_compile_definitions(${COMPONENT_LIB} PUBLIC EEZ_FOR_LVGL=1)