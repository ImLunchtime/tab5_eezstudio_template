#include <stdio.h>
#include "gui.h"
#include "hal.h"
#include "ui/ui.h"

void app_main(void) {
    // Initialize hardware
    hal_init();
    
    // Initialize touchpad
    hal_touchpad_init();

    // Initialize EEZ UI
    ui_init();

    bsp_display_unlock();
    
    // Main loop for UI updates
    while (1) {
        ui_tick();
        vTaskDelay(pdMS_TO_TICKS(5)); // 5ms delay for smooth UI updates
    }
}
