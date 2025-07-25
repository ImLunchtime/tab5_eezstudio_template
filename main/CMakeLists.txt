idf_component_register(SRCS "m5stack-tab5-lvgl.c"
                            "hal.c"
                            "gui.c"
                            "ui/ui.c"
                            "ui/screens.c"
                            "ui/images.c"
                            "ui/styles.c"
                            "ui/ui_image_miku1.c"
                    INCLUDE_DIRS "."
                                 "ui"
                    REQUIRES eez-framework lvgl esp_lcd_touch m5stack-tab5)

# Enable EEZ_FOR_LVGL flag
target_compile_definitions(${COMPONENT_LIB} PUBLIC EEZ_FOR_LVGL=1)