#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *screen2;
    lv_obj_t *screen3;
    lv_obj_t *button2;
    lv_obj_t *button2_1;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_SCREEN2 = 1,
    SCREEN_ID_SCREEN3 = 2,
};

void create_screen_screen2();
void tick_screen_screen2();

void create_screen_screen3();
void tick_screen_screen3();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/