/*
 * eez-framework
 *
 * MIT License
 * Copyright 2024 Envox d.o.o.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <stdint.h>

#include <eez/core/memory.h>
#include <eez/core/value.h>

namespace eez {

static const uint32_t HEADER_TAG = 0x5A45457E; // "~EEZ"
static const uint32_t HEADER_TAG_COMPRESSED = 0x7A65657E; // "~eez"

static const uint8_t PROJECT_VERSION_V2 = 2;
static const uint8_t PROJECT_VERSION_V3 = 3;

static const uint8_t ASSETS_TYPE_FIRMWARE = 1;
static const uint8_t ASSETS_TYPE_FIRMWARE_MODULE = 2;
static const uint8_t ASSETS_TYPE_RESOURCE = 3;
static const uint8_t ASSETS_TYPE_APPLET = 4;
static const uint8_t ASSETS_TYPE_DASHBOARD = 5;

struct Header {
	uint32_t tag; // HEADER_TAG or HEADER_TAG_COMPRESSED
	uint8_t projectMajorVersion;
	uint8_t projectMinorVersion;
	uint8_t assetsType;
    uint8_t reserved;
	uint32_t decompressedSize;
};

extern bool g_isMainAssetsLoaded;
struct Assets;
extern Assets *g_mainAssets;
extern bool g_mainAssetsUncompressed;
extern Assets *g_externalAssets;

////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct AssetsPtr {
    AssetsPtr() : offset(0) {}
    AssetsPtr(const AssetsPtr<T> &rhs) = delete;

	void operator=(T* ptr) {
		if (ptr != nullptr) {
            offset = (uint8_t *)ptr - (uint8_t *)&offset;
		} else {
			offset = 0;
		}
    }

    /* Conversion to a T pointer */
    operator T*() { return ptr(); }
    operator const T*() const { return ptr(); }
    /* Dereferencing operators */
          T* operator->()       { return ptr(); }
    const T* operator->() const { return ptr(); }

private:
    int32_t offset;

    T* ptr() {
        return offset ? (T *)((uint8_t *)&offset + offset) : nullptr;
    }

	const T* ptr() const {
		return offset ? (const T *)((uint8_t *)&offset + offset) : nullptr;
	}
};

////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct ListOfAssetsPtr {
	uint32_t count = 0;

    /* Array access */
    T*       operator[](uint32_t i)       { return item(i); }
    const T* operator[](uint32_t i) const { return item(i); }

private:
    AssetsPtr<AssetsPtr<T>> items;

    T* item(int i) {
        return static_cast<T *>(static_cast<AssetsPtr<T> *>(items)[i]);
    }

    const T* item(int i) const {
        return static_cast<const T *>(static_cast<const AssetsPtr<T> *>(items)[i]);
    }
};

////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct ListOfFundamentalType {
	uint32_t count;
    AssetsPtr<T> items;

    /* Array access */
    T&       operator[](uint32_t i)       { return ptr()[i]; }
    const T& operator[](uint32_t i) const { return ptr()[i]; }

private:
    T *ptr() {
        return static_cast<T *>(items);
    }
};

////////////////////////////////////////////////////////////////////////////////

struct Settings {
    uint16_t displayWidth;
    uint16_t displayHeight;
};

////////////////////////////////////////////////////////////////////////////////

#if EEZ_OPTION_GUI

#define WIDGET_FLAG_PIN_TO_LEFT (1 << 0)
#define WIDGET_FLAG_PIN_TO_RIGHT (1 << 1)
#define WIDGET_FLAG_PIN_TO_TOP (1 << 2)
#define WIDGET_FLAG_PIN_TO_BOTTOM (1 << 3)

#define WIDGET_FLAG_FIX_WIDTH (1 << 4)
#define WIDGET_FLAG_FIX_HEIGHT (1 << 5)

#define WIDGET_TIMELINE_PROPERTY_X (1 << 0)
#define WIDGET_TIMELINE_PROPERTY_Y (1 << 1)
#define WIDGET_TIMELINE_PROPERTY_WIDTH (1 << 2)
#define WIDGET_TIMELINE_PROPERTY_HEIGHT (1 << 3)
#define WIDGET_TIMELINE_PROPERTY_OPACITY (1 << 4)
#define WIDGET_TIMELINE_PROPERTY_CP1 (1 << 5)
#define WIDGET_TIMELINE_PROPERTY_CP2 (1 << 6)

#define EASING_FUNC_LINEAR 0
#define EASING_FUNC_IN_QUAD 1
#define EASING_FUNC_OUT_QUAD 2
#define EASING_FUNC_IN_OUT_QUAD 3
#define EASING_FUNC_IN_CUBIC 4
#define EASING_FUNC_OUT_CUBIC 5
#define EASING_FUNC_IN_OUT_CUBIC 6
#define EASING_FUNC_IN__QUART 7
#define EASING_FUNC_OUT_QUART 8
#define EASING_FUNC_IN_OUT_QUART 9
#define EASING_FUNC_IN_QUINT 10
#define EASING_FUNC_OUT_QUINT 11
#define EASING_FUNC_IN_OUT_QUINT 12
#define EASING_FUNC_IN_SINE 13
#define EASING_FUNC_OUT_SINE 14
#define EASING_FUNC_IN_OUT_SINE 15
#define EASING_FUNC_IN_EXPO 16
#define EASING_FUNC_OUT_EXPO 17
#define EASING_FUNC_IN_OUT_EXPO 18
#define EASING_FUNC_IN_CIRC 19
#define EASING_FUNC_OUT_CIRC 20
#define EASING_FUNC_IN_OUT_CIRC 21
#define EASING_FUNC_IN_BACK 22
#define EASING_FUNC_OUT_BACK 23
#define EASING_FUNC_IN_OUT_BACK 24
#define EASING_FUNC_IN_ELASTIC 25
#define EASING_FUNC_OUT_ELASTIC 26
#define EASING_FUNC_IN_OUT_ELASTIC 27
#define EASING_FUNC_IN_BOUNCE 28
#define EASING_FUNC_OUT_BOUNCE 29
#define EASING_FUNC_IN_OUT_BOUNCE 30

namespace gui {

struct TimelineKeyframe {
    float start;
    float end;

    uint32_t enabledProperties;

	int16_t x;
	int16_t y;

	int16_t width;
	int16_t height;

    float opacity;

    uint8_t xEasingFunc;
    uint8_t yEasingFunc;
    uint8_t widthEasingFunc;
    uint8_t heightEasingFunc;

    uint8_t opacityEasingFunc;
    uint8_t reserved1;
    uint16_t reserved2;

	int16_t cp1x;
	int16_t cp1y;
    int16_t cp2x;
    int16_t cp2y;
};

struct Widget {
	uint16_t type;
	int16_t data;
    int16_t visible;
	int16_t action;
	int16_t x;
	int16_t y;
	int16_t width;
	int16_t height;
	int16_t style;
    uint16_t flags;
    ListOfAssetsPtr<TimelineKeyframe> timeline;
};

#define SHADOW_FLAG (1 << 0)
#define CLOSE_PAGE_IF_TOUCHED_OUTSIDE_FLAG (1 << 1)
#define PAGE_IS_USED_AS_USER_WIDGET (1 << 2)
#define PAGE_CONTAINER (1 << 3)
#define PAGE_SCALE_TO_FIT (1 << 4)

struct PageAsset : public Widget {
	ListOfAssetsPtr<Widget> widgets;
	uint16_t flags;
	int16_t overlay;
};

////////////////////////////////////////////////////////////////////////////////

#define STYLE_FLAGS_HORZ_ALIGN_MASK 0x7
#define STYLE_FLAGS_HORZ_ALIGN_LEFT 0
#define STYLE_FLAGS_HORZ_ALIGN_RIGHT 1
#define STYLE_FLAGS_HORZ_ALIGN_CENTER 2

#define STYLE_FLAGS_VERT_ALIGN_MASK (0x7 << 3)
#define STYLE_FLAGS_VERT_ALIGN_TOP (0 << 3)
#define STYLE_FLAGS_VERT_ALIGN_BOTTOM (1 << 3)
#define STYLE_FLAGS_VERT_ALIGN_CENTER (2 << 3)

#define STYLE_FLAGS_BLINK (1 << 6)

struct Style {
    uint16_t flags; // STYLE_FLAGS_...

    uint16_t backgroundColor;
    uint16_t color;

    uint16_t activeBackgroundColor;
    uint16_t activeColor;

    uint16_t focusBackgroundColor;
    uint16_t focusColor;

    uint8_t borderSizeTop;
    uint8_t borderSizeRight;
    uint8_t borderSizeBottom;
    uint8_t borderSizeLeft;

    uint16_t borderColor;

    uint8_t borderRadiusTLX;
	uint8_t borderRadiusTLY;
    uint8_t borderRadiusTRX;
	uint8_t borderRadiusTRY;
    uint8_t borderRadiusBLX;
	uint8_t borderRadiusBLY;
    uint8_t borderRadiusBRX;
	uint8_t borderRadiusBRY;

    uint8_t font;
    uint8_t opacity; // 0 - 255

    uint8_t paddingTop;
    uint8_t paddingRight;
    uint8_t paddingBottom;
    uint8_t paddingLeft;

	int16_t backgroundImage;
};

////////////////////////////////////////////////////////////////////////////////

struct GlyphData {
	int8_t dx;         // DWIDTH (-128 indicated empty glyph)
	uint8_t width;     // BBX width
	uint8_t height;    // BBX height
	int8_t x;          // BBX xoffset
	int8_t y;          // BBX yoffset
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
	uint8_t pixels[1];
};

struct GlyphsGroup {
    uint32_t encoding;
    uint32_t glyphIndex;
    uint32_t length;
};

struct FontData {
	uint8_t ascent;
	uint8_t descent;
    uint8_t reserved1;
    uint8_t reserved2;
	uint32_t encodingStart;
	uint32_t encodingEnd;
    ListOfAssetsPtr<GlyphsGroup> groups;
    ListOfAssetsPtr<GlyphData> glyphs;
};

////////////////////////////////////////////////////////////////////////////////

struct Bitmap {
    int16_t w;
    int16_t h;
    int16_t bpp;
    int16_t reserved;
    AssetsPtr<const char> name;
    const uint8_t pixels[1];
};

} // namespace gui

#endif // EEZ_OPTION_GUI

////////////////////////////////////////////////////////////////////////////////

struct Theme {
	AssetsPtr<const char> name;
	ListOfFundamentalType<uint16_t> colors;
};

struct Colors {
	ListOfAssetsPtr<Theme> themes;
	ListOfFundamentalType<uint16_t> colors;
};

////////////////////////////////////////////////////////////////////////////////

static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_MASK  = 0x0007 << 13;
static const uint16_t EXPR_EVAL_INSTRUCTION_PARAM_MASK = 0xFFFF >> 3;

static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_PUSH_CONSTANT           = (0 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_PUSH_INPUT              = (1 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_PUSH_LOCAL_VAR          = (2 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_PUSH_GLOBAL_VAR         = (3 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_PUSH_OUTPUT             = (4 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_ARRAY_ELEMENT                = (5 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_OPERATION               = (6 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_END                     = (7 << 13);
static const uint16_t EXPR_EVAL_INSTRUCTION_TYPE_END_WITH_DST_VALUE_TYPE = (7 << 13) | (1 << 12);

struct Property {
	uint8_t evalInstructions[1];
};

struct Connection {
	uint16_t targetComponentIndex;
	uint16_t targetInputIndex;
};

struct ComponentOutput {
	ListOfAssetsPtr<Connection> connections;
	uint32_t isSeqOut;
};

static const uint16_t BREAKPOINT_ENABLED = 1;
static const uint16_t BREAKPOINT_DISABLED = 2;

struct Component {
    uint16_t type;
    uint16_t breakpoint;

	// These are indexes to Flow::componentInputs.
	// We use this to check if component is ready to run (i.e. all mandatory inputs have a value).
	ListOfFundamentalType<uint16_t> inputs;

	ListOfAssetsPtr<Property> properties;
	ListOfAssetsPtr<ComponentOutput> outputs;
	int16_t errorCatchOutput;
	uint16_t reserved;
};

struct WidgetDataItem {
	int16_t componentIndex;
	int16_t propertyValueIndex;
};

struct WidgetActionItem {
	int16_t componentIndex;
	int16_t componentOutputIndex;
};

#define COMPONENT_INPUT_FLAG_IS_SEQ_INPUT   (1 << 0)
#define COMPONENT_INPUT_FLAG_IS_OPTIONAL (1 << 1)
typedef uint8_t ComponentInput;

struct Flow {
	ListOfAssetsPtr<Component> components;
	ListOfAssetsPtr<Value> localVariables;

	// List of all component inputs of all components in this flow
	// When flow state is created we reserve this many Value's in memory
	// to keep the latest value of component input.
	ListOfFundamentalType<ComponentInput> componentInputs;

	ListOfAssetsPtr<WidgetDataItem> widgetDataItems;
	ListOfAssetsPtr<WidgetActionItem> widgetActions;

    ListOfFundamentalType<uint8_t> userPropertiesAssignable;
};

struct FlowDefinition {
	ListOfAssetsPtr<Flow> flows;
	ListOfAssetsPtr<Value> constants;
	ListOfAssetsPtr<Value> globalVariables;
};

struct Language {
    AssetsPtr<const char> languageID;
    ListOfAssetsPtr<const char> translations;
};

////////////////////////////////////////////////////////////////////////////////

struct Assets {
    uint8_t projectMajorVersion;
	uint8_t projectMinorVersion;
    uint8_t assetsType;
    uint8_t external;

    uint32_t reserved;

    AssetsPtr<Settings> settings;
#if EEZ_OPTION_GUI
	ListOfAssetsPtr<gui::PageAsset> pages;
	ListOfAssetsPtr<gui::Style> styles;
	ListOfAssetsPtr<gui::FontData> fonts;
	ListOfAssetsPtr<gui::Bitmap> bitmaps;
#endif // EEZ_OPTION_GUI
	AssetsPtr<Colors> colorsDefinition;
	ListOfAssetsPtr<const char> actionNames;
	ListOfAssetsPtr<const char> variableNames;
	AssetsPtr<FlowDefinition> flowDefinition;
    ListOfAssetsPtr<Language> languages;
};

////////////////////////////////////////////////////////////////////////////////

bool decompressAssetsData(const uint8_t *assetsData, uint32_t assetsDataSize, Assets *decompressedAssets, uint32_t maxDecompressedAssetsSize, int *err);

////////////////////////////////////////////////////////////////////////////////

void loadMainAssets(const uint8_t *assets, uint32_t assetsSize);
bool loadExternalAssets(const char *filePath, int *err);
void unloadExternalAssets();

////////////////////////////////////////////////////////////////////////////////

#if EEZ_OPTION_GUI
const gui::PageAsset *getPageAsset(int pageId);
const gui::PageAsset* getPageAsset(int pageId, gui::WidgetCursor& widgetCursor);
const gui::Style *getStyle(int styleID);
const gui::FontData *getFontData(int fontID);
const gui::Bitmap *getBitmap(int bitmapID);
const int getBitmapIdByName(const char *bitmapName);
#endif

int getThemesCount();
const char *getThemeName(int i);
uint32_t getThemeColorsCount(int themeIndex);
const uint16_t *getThemeColors(int themeIndex);
const uint16_t *getColors();

int getExternalAssetsMainPageId();

#if EEZ_OPTION_GUI
const char *getActionName(const gui::WidgetCursor &widgetCursor, int16_t actionId);
int16_t getDataIdFromName(const gui::WidgetCursor &widgetCursor, const char *name);
#endif

////////////////////////////////////////////////////////////////////////////////

} // namespace eez
