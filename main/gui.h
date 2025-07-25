#pragma once

#include "lvgl.h"

/**
 * @brief Initialize the GUI with a centered button
 * 
 * @param disp LVGL display instance
 */
void gui_init(lv_disp_t *disp);

/**
 * @brief Initialize EEZ UI system
 * This function is called to set up the EEZ Studio generated UI
 */
void eez_ui_init(void);

/**
 * @brief Update EEZ UI system
 * This function should be called periodically to update the UI
 */
void eez_ui_tick(void);