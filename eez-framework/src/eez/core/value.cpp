/*
 * eez-framework
 *
 * MIT License
 * Copyright 2024 Envox d.o.o.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <eez/conf-internal.h>

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

#include <eez/core/util.h>
#include <eez/core/value.h>
#include <eez/core/vars.h>

#include <eez/flow/flow.h>
#include <eez/flow/hooks.h>
#include <eez/flow/date.h>
#include <eez/flow/expression.h>
#include <eez/flow/private.h>

#if defined(EEZ_DASHBOARD_API)
#include <eez/flow/dashboard_api.h>
#endif

namespace eez {

////////////////////////////////////////////////////////////////////////////////

static bool compare_UNDEFINED_value(const Value &a, const Value &b) {
    return b.type == VALUE_TYPE_UNDEFINED && a.int32Value == b.int32Value;
}

static void UNDEFINED_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    *text = 0;
}

static const char *UNDEFINED_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "undefined";
}

static bool compare_NULL_value(const Value &a, const Value &b) {
    EEZ_UNUSED(a);
    return b.type == VALUE_TYPE_NULL;
}

static void NULL_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    *text = 0;
}

static const char *NULL_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "null";
}

static bool compare_BOOLEAN_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getInt() == b.getInt();
}

static void BOOLEAN_value_to_text(const Value &value, char *text, int count) {
    if (value.getInt()) {
        stringCopy(text, count, "true");
    } else {
        stringCopy(text, count, "false");
    }
}

static const char *BOOLEAN_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "boolean";
}

static bool compare_INT8_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getInt8() == b.getInt8();
}

static void INT8_value_to_text(const Value &value, char *text, int count) {
    stringAppendInt(text, count, value.getInt8());
}

static const char *INT8_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "int8";
}

static bool compare_UINT8_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUInt8() == b.getUInt8();
}

static void UINT8_value_to_text(const Value &value, char *text, int count) {
    stringAppendUInt32(text, count, value.getUInt8());
}

static const char *UINT8_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "uint8";
}

static bool compare_INT16_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getInt16() == b.getInt16();
}

static void INT16_value_to_text(const Value &value, char *text, int count) {
    stringAppendInt(text, count, value.getInt16());
}

static const char *INT16_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "int16";
}

static bool compare_UINT16_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUInt16() == b.getUInt16();
}

static void UINT16_value_to_text(const Value &value, char *text, int count) {
    stringAppendUInt32(text, count, value.getUInt16());
}

static const char *UINT16_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "uint16";
}

static bool compare_INT32_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getInt32() == b.getInt32();
}

static void INT32_value_to_text(const Value &value, char *text, int count) {
    stringAppendInt(text, count, value.getInt32());
}

static const char *INT32_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "int32";
}

static bool compare_UINT32_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUInt32() == b.getUInt32();
}

static void UINT32_value_to_text(const Value &value, char *text, int count) {
    stringAppendUInt32(text, count, value.getUInt32());
}

static const char *UINT32_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "uint32";
}

static bool compare_INT64_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getInt64() == b.getInt64();
}

static void INT64_value_to_text(const Value &value, char *text, int count) {
    stringAppendInt64(text, count, value.getInt64());
}

static const char *INT64_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "int64";
}

static bool compare_UINT64_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUInt64() == b.getUInt64();
}

static void UINT64_value_to_text(const Value &value, char *text, int count) {
    stringAppendUInt64(text, count, value.getUInt64());
}

static const char *UINT64_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "uint64";
}

static bool compare_FLOAT_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUnit() == b.getUnit() && a.getFloat() == b.getFloat() && a.getOptions() == b.getOptions();
}

static void FLOAT_value_to_text(const Value &value, char *text, int count) {
    text[0] = 0;

    float floatValue = value.getFloat();

#if defined(INFINITY_SYMBOL)
    if (isinf(floatValue)) {
        snprintf(text, count, INFINITY_SYMBOL);
        return;
    }
#endif

    Unit unit = value.getUnit();

    bool appendDotZero = unit == UNIT_VOLT || unit == UNIT_VOLT_PP || unit == UNIT_AMPER || unit == UNIT_AMPER_PP || unit == UNIT_WATT;

    uint16_t options = value.getOptions();
    bool fixedDecimals = (options & FLOAT_OPTIONS_FIXED_DECIMALS) != 0;

    if (floatValue != 0) {
        if (!fixedDecimals) {
            unit = findDerivedUnit(floatValue, unit);
            floatValue /= getUnitFactor(unit);
        }
    } else {
        floatValue = 0; // set to zero just in case we have negative zero
    }

    if (!isNaN(floatValue)) {
        if ((value.getOptions() & FLOAT_OPTIONS_LESS_THEN) != 0) {
            stringAppendString(text, count, "< ");
			appendDotZero = false;
        }

        if (fixedDecimals) {
            stringAppendFloat(text, count, floatValue, FLOAT_OPTIONS_GET_NUM_FIXED_DECIMALS(options));
        } else {
            if (unit == UNIT_WATT || unit == UNIT_MILLI_WATT) {
                stringAppendFloat(text, count, floatValue, 2);
            } else {
                stringAppendFloat(text, count, floatValue);
            }

            int n = strlen(text);

            int decimalPointIndex;
            for (decimalPointIndex = 0; decimalPointIndex < n; ++decimalPointIndex) {
                if (text[decimalPointIndex] == '.') {
                    break;
                }
            }

            if (decimalPointIndex == n) {
                if (appendDotZero) {
                    // 1 => 1.0
                    stringAppendString(text, count, ".0");
                }
            } else if (decimalPointIndex == n - 1) {
                if (appendDotZero) {
                    // 1. => 1.0
                    stringAppendString(text, count, "0");
                } else {
                    text[decimalPointIndex] = 0;
                }
            } else {
                // remove trailing zeros
                if (appendDotZero) {
                    for (int j = n - 1; j > decimalPointIndex + 1 && text[j] == '0'; j--) {
                        text[j] = 0;
                    }
                } else {
                    for (int j = n - 1; j >= decimalPointIndex && (text[j] == '0' || text[j] == '.'); j--) {
                        text[j] = 0;
                    }
                }
            }
        }

        const char *unitName = getUnitName(unit);
        if (unitName && *unitName) {
            stringAppendString(text, count, " ");
            stringAppendString(text, count, unitName);
        }
    } else {
        text[0] = 0;
    }
}

static const char *FLOAT_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "float";
}

static bool compare_DOUBLE_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUnit() == b.getUnit() && a.getDouble() == b.getDouble() && a.getOptions() == b.getOptions();
}

static void DOUBLE_value_to_text(const Value &value, char *text, int count) {
    text[0] = 0;

    double doubleValue = value.getDouble();

#if defined(INFINITY_SYMBOL)
    if (isinf(doubleValue)) {
        snprintf(text, count, INFINITY_SYMBOL);
        return;
    }
#endif

    Unit unit = value.getUnit();

    bool appendDotZero = unit == UNIT_VOLT || unit == UNIT_VOLT_PP || unit == UNIT_AMPER || unit == UNIT_AMPER_PP || unit == UNIT_WATT;

    uint16_t options = value.getOptions();
    bool fixedDecimals = (options & FLOAT_OPTIONS_FIXED_DECIMALS) != 0;

    if (doubleValue != 0) {
        if (!fixedDecimals) {
            unit = findDerivedUnit(fabs(doubleValue), unit);
            doubleValue /= getUnitFactor(unit);
        }
    } else {
        doubleValue = 0; // set to zero just in case we have negative zero
    }

    if (!isNaN(doubleValue)) {
        if ((value.getOptions() & FLOAT_OPTIONS_LESS_THEN) != 0) {
            stringAppendString(text, count, "< ");
			appendDotZero = false;
        }

        if (fixedDecimals) {
            stringAppendFloat(text, count, doubleValue, FLOAT_OPTIONS_GET_NUM_FIXED_DECIMALS(options));
        } else {
            if (unit == UNIT_WATT || unit == UNIT_MILLI_WATT) {
                stringAppendDouble(text, count, doubleValue, 2);
            } else {
                stringAppendDouble(text, count, doubleValue);
            }

            int n = strlen(text);

            int decimalPointIndex;
            for (decimalPointIndex = 0; decimalPointIndex < n; ++decimalPointIndex) {
                if (text[decimalPointIndex] == '.') {
                    break;
                }
            }

            if (decimalPointIndex == n) {
                if (appendDotZero) {
                    // 1 => 1.0
                    stringAppendString(text, count, ".0");
                }
            } else if (decimalPointIndex == n - 1) {
                if (appendDotZero) {
                    // 1. => 1.0
                    stringAppendString(text, count, "0");
                } else {
                    text[decimalPointIndex] = 0;
                }
            } else {
                // remove trailing zeros
                if (appendDotZero) {
                    for (int j = n - 1; j > decimalPointIndex + 1 && text[j] == '0'; j--) {
                        text[j] = 0;
                    }
                } else {
                    for (int j = n - 1; j >= decimalPointIndex && (text[j] == '0' || text[j] == '.'); j--) {
                        text[j] = 0;
                    }
                }
            }
        }

        const char *unitName = getUnitName(unit);
        if (unitName && *unitName) {
            stringAppendString(text, count, " ");
            stringAppendString(text, count, unitName);
        }
    } else {
        text[0] = 0;
    }
}

static const char *DOUBLE_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "double";
}

static bool compare_STRING_value(const Value &a, const Value &b) {
    if (!b.isString()) {
        return false;
    }
    const char *astr = a.getString();
    const char *bstr = b.getString();
    if (!astr && !bstr) {
        return true;
    }
    if ((!astr && bstr) || (astr && !bstr)) {
        return false;
    }
    return strcmp(astr, bstr) == 0;
}

static void STRING_value_to_text(const Value &value, char *text, int count) {
    const char *str = value.getString();
    if (str) {
        stringCopy(text, count, str);
    } else {
        text[0] = 0;
    }
}

static const char *STRING_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "string";
}

static bool compare_STRING_ASSET_value(const Value &a, const Value &b) {
    return compare_STRING_value(a, b);
}

static void STRING_ASSET_value_to_text(const Value &value, char *text, int count) {
    STRING_value_to_text(value, text, count);
}

static const char *STRING_ASSET_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "string";
}

static bool compare_ARRAY_value(const Value &a, const Value &b) {
    return a.type == b.type && a.arrayValue == b.arrayValue;
}

static void ARRAY_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    text[0] = 0;
}

static const char *ARRAY_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "array";
}

static bool compare_ARRAY_ASSET_value(const Value &a, const Value &b) {
    return a.type == b.type && a.int32Value == b.int32Value;
}

static void ARRAY_ASSET_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    text[0] = 0;
}

static const char *ARRAY_ASSET_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "array";
}

static bool compare_ARRAY_REF_value(const Value &a, const Value &b) {
    return a.type == b.type && a.refValue == b.refValue;
}

static void ARRAY_REF_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    text[0] = 0;
}

static const char *ARRAY_REF_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "array";
}

static bool compare_STRING_REF_value(const Value &a, const Value &b) {
	return compare_STRING_value(a, b);
}

static void STRING_REF_value_to_text(const Value &value, char *text, int count) {
	STRING_value_to_text(value, text, count);
}

static const char *STRING_REF_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "string";
}

static bool compare_BLOB_REF_value(const Value &a, const Value &b) {
    return a.type == b.type && a.refValue == b.refValue;
}

static void BLOB_REF_value_to_text(const Value &value, char *text, int count) {
    snprintf(text, count, "blob (size=%d)", value.getInt());
}

static const char *BLOB_REF_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "blob";
}

static bool compare_STREAM_value(const Value &a, const Value &b) {
    return a.type == b.type && a.int32Value == b.int32Value;
}

static void STREAM_value_to_text(const Value &value, char *text, int count) {
    snprintf(text, count, "stream (id=%d)", value.getInt());
}

static const char *STREAM_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "stream";
}

static bool compare_WIDGET_value(const Value &a, const Value &b) {
#if defined(EEZ_FOR_LVGL)
    return a.type == b.type && a.getVoidPointer() == b.getVoidPointer();
#else
    return a.type == b.type && a.int32Value == b.int32Value;
#endif
}

static void WIDGET_value_to_text(const Value &value, char *text, int count) {
#if defined(EEZ_FOR_LVGL)
    snprintf(text, count, "widget (%p)", value.getVoidPointer());
#else
    snprintf(text, count, "widget (id=%d)", value.getInt());
#endif
}

static const char *WIDGET_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "widget";
}

static bool compare_JSON_value(const Value &a, const Value &b) {
    return a.type == b.type && a.int32Value == b.int32Value;
}

static void JSON_value_to_text(const Value &value, char *text, int count) {
    snprintf(text, count, "json (id=%d)", value.getInt());
}

static const char *JSON_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "json";
}

static bool compare_JSON_MEMBER_VALUE_value(const Value &a, const Value &b) {
	return a.getValue() == b.getValue();
}

static void JSON_MEMBER_VALUE_value_to_text(const Value &value, char *text, int count) {
	value.getValue().toText(text, count);
}

static const char *JSON_MEMBER_VALUE_value_type_name(const Value &value) {
    auto value2 = value.getValue();
    return g_valueTypeNames[value2.type](value2);
}

static bool compare_EVENT_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getVoidPointer() == b.getVoidPointer();
}

static void EVENT_value_to_text(const Value &value, char *text, int count) {
    snprintf(text, count, "event (%p)", value.getVoidPointer());
}

static const char *EVENT_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "event";
}

static const char *PROPERTY_REF_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "property-ref";
}

static bool compare_PROPERTY_REF_value(const Value &a, const Value &b) {
    return a.type == b.type && a.refValue == b.refValue;
}

static void PROPERTY_REF_value_to_text(const Value &value, char *text, int count) {
    snprintf(text, count, "property-ref (flowState=%p, component=%d, property=%d)",
        (void *)value.getPropertyRef()->flowState, value.getPropertyRef()->componentIndex, value.getPropertyRef()->propertyIndex);
}

static bool compare_DATE_value(const Value &a, const Value &b) {
    return a.type == b.type && a.doubleValue == b.doubleValue;
}

static void DATE_value_to_text(const Value &value, char *text, int count) {
    flow::date::toLocaleString((flow::date::Date)value.getDouble(), text, count);
}

static const char *DATE_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "date";
}

static bool compare_VERSIONED_STRING_value(const Value &a, const Value &b) {
    return a.type == b.type && a.unit == b.unit; // here unit is used as string version
}

static void VERSIONED_STRING_value_to_text(const Value &value, char *text, int count) {
    const char *str = value.getString();
    if (str) {
        stringCopy(text, count, str);
    } else {
        text[0] = 0;
    }
}

static const char *VERSIONED_STRING_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "versioned-string";
}

static bool compare_VALUE_PTR_value(const Value &a, const Value &b) {
	return a.type == b.type && (a.pValueValue == b.pValueValue || (a.pValueValue && b.pValueValue && *a.pValueValue == *b.pValueValue));
}

static void VALUE_PTR_value_to_text(const Value &value, char *text, int count) {
	if (value.pValueValue) {
		value.pValueValue->toText(text, count);
	} else {
		text[0] = 0;
	}
}

static const char *VALUE_PTR_value_type_name(const Value &value) {
	if (value.pValueValue) {
		return g_valueTypeNames[value.pValueValue->type](value.pValueValue);
	} else {
		return "null";
	}
}

static bool compare_ARRAY_ELEMENT_VALUE_value(const Value &a, const Value &b) {
	return a.getValue() == b.getValue();
}

static void ARRAY_ELEMENT_VALUE_value_to_text(const Value &value, char *text, int count) {
	value.getValue().toText(text, count);
}

static const char *ARRAY_ELEMENT_VALUE_value_type_name(const Value &value) {
    auto value2 = value.getValue();
    return g_valueTypeNames[value2.type](value2);
}

static bool compare_FLOW_OUTPUT_value(const Value &a, const Value &b) {
	return a.type == b.type && a.getUInt16() == b.getUInt16();
}

static void FLOW_OUTPUT_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    text[0] = 0;
}

static const char *FLOW_OUTPUT_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "internal";
}

#if EEZ_OPTION_GUI
using namespace gui;
static bool compare_NATIVE_VARIABLE_value(const Value &a, const Value &b) {
    auto aValue = get(g_widgetCursor, a.getInt());
    auto bValue = get(g_widgetCursor, b.getInt());
	return aValue == bValue;
}

static void NATIVE_VARIABLE_value_to_text(const Value &value, char *text, int count) {
    auto aValue = get(g_widgetCursor, value.getInt());
    aValue.toText(text, count);
}

static const char *NATIVE_VARIABLE_value_type_name(const Value &value) {
    auto aValue = get(g_widgetCursor, value.getInt());
    return g_valueTypeNames[aValue.type](aValue);
}
#else

static bool compare_NATIVE_VARIABLE_value(const Value &a, const Value &b) {
    EEZ_UNUSED(a);
    EEZ_UNUSED(b);
    return false;
}

static void NATIVE_VARIABLE_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    *text = 0;
}

static const char *NATIVE_VARIABLE_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "";
}
#endif

static bool compare_ERROR_value(const Value &a, const Value &b) {
    EEZ_UNUSED(a);
    EEZ_UNUSED(b);
	return false;
}

static void ERROR_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    *text = 0;
}

static const char *ERROR_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "error";
}

static bool compare_RANGE_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUInt32() == b.getUInt32();
}

static void RANGE_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    text[0] = 0;
}

static const char *RANGE_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "internal";
}

static bool compare_POINTER_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getVoidPointer() == b.getVoidPointer();
}

static void POINTER_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    text[0] = 0;
}

static const char *POINTER_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "internal";
}

#if EEZ_OPTION_GUI
using namespace gui;
static bool compare_ENUM_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getEnum().enumDefinition == b.getEnum().enumDefinition &&
           a.getEnum().enumValue == b.getEnum().enumValue;
}

static void ENUM_value_to_text(const Value &value, char *text, int count) {
    const EnumItem *enumDefinition = g_enumDefinitions[value.getEnum().enumDefinition];
    for (int i = 0; enumDefinition[i].menuLabel; ++i) {
        if (value.getEnum().enumValue == enumDefinition[i].value) {
            if (enumDefinition[i].widgetLabel) {
                stringCopy(text, count, enumDefinition[i].widgetLabel);
            } else {
                stringCopy(text, count, enumDefinition[i].menuLabel);
            }
            break;
        }
    }
}

static const char *ENUM_value_type_name(const Value &value) {
    return "internal";
}
#else

static bool compare_ENUM_value(const Value &a, const Value &b) {
    EEZ_UNUSED(a);
    EEZ_UNUSED(b);
    return false;
}

static void ENUM_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    *text = 0;
}

static const char *ENUM_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "internal";
}

#endif // EEZ_OPTION_GUI

static bool compare_YT_DATA_GET_VALUE_FUNCTION_POINTER_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUInt32() == b.getUInt32();
}

static bool compare_IP_ADDRESS_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getUInt32() == b.getUInt32();
}

static void IP_ADDRESS_value_to_text(const Value &value, char *text, int count) {
    ipAddressToString(value.getUInt32(), text, count);
}

static const char *IP_ADDRESS_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "internal";
}

static bool compare_TIME_ZONE_value(const Value &a, const Value &b) {
    return a.type == b.type && a.getInt16() == b.getInt16();
}

static void TIME_ZONE_value_to_text(const Value &value, char *text, int count) {
    formatTimeZone(value.getInt16(), text, count);
}

static const char *TIME_ZONE_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "internal";
}

static void YT_DATA_GET_VALUE_FUNCTION_POINTER_value_to_text(const Value &value, char *text, int count) {
    EEZ_UNUSED(value);
    EEZ_UNUSED(count);
    text[0] = 0;
}

static const char *YT_DATA_GET_VALUE_FUNCTION_POINTER_value_type_name(const Value &value) {
    EEZ_UNUSED(value);
    return "internal";
}

////////////////////////////////////////////////////////////////////////////////

#define VALUE_TYPE(NAME) bool compare_##NAME##_value(const Value &a, const Value &b);
VALUE_TYPES
#undef VALUE_TYPE
#define VALUE_TYPE(NAME) compare_##NAME##_value,
CompareValueFunction g_valueTypeCompareFunctions[] = {
	VALUE_TYPES
};
#undef VALUE_TYPE

#define VALUE_TYPE(NAME) void NAME##_value_to_text(const Value &value, char *text, int count);
VALUE_TYPES
#undef VALUE_TYPE
#define VALUE_TYPE(NAME) NAME##_value_to_text,
ValueToTextFunction g_valueTypeToTextFunctions[] = {
	VALUE_TYPES
};
#undef VALUE_TYPE

#define VALUE_TYPE(NAME) const char * NAME##_value_type_name(const Value &value);
VALUE_TYPES
#undef VALUE_TYPE
#define VALUE_TYPE(NAME) NAME##_value_type_name,
ValueTypeNameFunction g_valueTypeNames[] = {
	VALUE_TYPES
};
#undef VALUE_TYPE

////////////////////////////////////////////////////////////////////////////////

ArrayValueRef::~ArrayValueRef() {
    eez::flow::onArrayValueFree(&arrayValue);
    for (uint32_t i = 1; i < arrayValue.arraySize; i++) {
        (arrayValue.values + i)->~Value();
    }
}

////////////////////////////////////////////////////////////////////////////////

bool assignValue(Value &dstValue, const Value &srcValue, uint32_t dstValueType) {
    // if (srcValue.type != dstValue.type) {
    //     printf("%s (%d) <- %s (%d)\n", dstValue.toString(0).getString(), dstValue.type, srcValue.toString(0).getString(), srcValue.type);
    // }

    if (dstValueType == VALUE_TYPE_BOOLEAN) {
        dstValue = Value(srcValue.toBool(), VALUE_TYPE_BOOLEAN);
    } else if (Value::isInt32OrLess(dstValueType)) {
        dstValue = Value((int)srcValue.toInt32(), (ValueType)dstValueType);
    } else if (dstValueType == VALUE_TYPE_FLOAT) {
        dstValue = Value(srcValue.toFloat(), VALUE_TYPE_FLOAT);
    } else if (dstValueType == VALUE_TYPE_DOUBLE) {
        dstValue = Value(srcValue.toDouble(), VALUE_TYPE_DOUBLE);
    } else if (dstValueType == VALUE_TYPE_STRING) {
        dstValue = srcValue.toString(0x30a91156);
#if defined(EEZ_DASHBOARD_API)
    } else if (dstValueType == VALUE_TYPE_JSON) {
        if (srcValue.isJson()) {
            dstValue = srcValue;
        } else {
            dstValue = flow::convertToJson(&srcValue);
        }
    } else if (srcValue.isJson()) {
        if (dstValueType == VALUE_TYPE_JSON) {
            dstValue = srcValue;
        }  else {
            dstValue = flow::convertFromJson(srcValue.getInt(), dstValueType);
        }
#endif
    } else if (dstValue.isBoolean()) {
        dstValue.int32Value = srcValue.toBool();
    } else if (dstValue.isInt32OrLess()) {
        dstValue.int32Value = srcValue.toInt32();
    } else if (dstValue.isFloat()) {
        dstValue.floatValue = srcValue.toFloat();
    } else if (dstValue.isDouble()) {
        dstValue.doubleValue = srcValue.toDouble();
    } else if (dstValue.isString()) {
        dstValue = srcValue.toString(0x30a91156);
    } else {
        dstValue = srcValue;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

uint16_t getPageIndexFromValue(const Value &value) {
    return value.getFirstUInt16();
}

uint16_t getNumPagesFromValue(const Value &value) {
    return value.getSecondUInt16();
}

////////////////////////////////////////////////////////////////////////////////

Value MakeRangeValue(uint16_t from, uint16_t to) {
    Value value;
    value.type = VALUE_TYPE_RANGE;
    value.pairOfUint16Value.first = from;
    value.pairOfUint16Value.second = to;
    return value;
}

Value MakeEnumDefinitionValue(uint8_t enumValue, uint8_t enumDefinition) {
    Value value;
    value.type = VALUE_TYPE_ENUM;
    value.enumValue.enumValue = enumValue;
    value.enumValue.enumDefinition = enumDefinition;
    return value;
}

////////////////////////////////////////////////////////////////////////////////

const char *Value::getString() const {
    auto value = getValue(); // will convert VALUE_TYPE_STRING_ASSET to VALUE_TYPE_STRING by using copy constructor
	if (value.type == VALUE_TYPE_STRING_REF) {
		return ((StringRef *)value.refValue)->str;
	}
	if (value.type == VALUE_TYPE_STRING) {
		return value.strValue;
	}
	return nullptr;
}

const ArrayValue *Value::getArray() const {
    if (type == VALUE_TYPE_ARRAY) {
        return arrayValue;
    }
    if (type == VALUE_TYPE_ARRAY_ASSET) {
        return (ArrayValue *)((uint8_t *)&int32Value + int32Value);
    }
    return &((ArrayValueRef *)refValue)->arrayValue;
}

ArrayValue *Value::getArray() {
    if (type == VALUE_TYPE_ARRAY) {
        return arrayValue;
    }
    if (type == VALUE_TYPE_ARRAY_ASSET) {
        return (ArrayValue *)((uint8_t *)&int32Value + int32Value);
    }
    return &((ArrayValueRef *)refValue)->arrayValue;
}

double Value::toDouble(int *err) const {
	if (isIndirectValueType()) {
		return getValue().toDouble(err);
	}

	if (err) {
		*err = 0;
	}

	if (type == VALUE_TYPE_DOUBLE) {
		return doubleValue;
	}

	if (type == VALUE_TYPE_FLOAT) {
		return floatValue;
	}

	if (type == VALUE_TYPE_INT8) {
		return int8Value;
	}
	if (type == VALUE_TYPE_UINT8) {
		return uint8Value;
	}

	if (type == VALUE_TYPE_INT16) {
		return int16Value;
	}
	if (type == VALUE_TYPE_UINT16) {
		return uint16Value;
	}

	if (type == VALUE_TYPE_INT32 || type == VALUE_TYPE_BOOLEAN) {
		return int32Value;
	}
	if (type == VALUE_TYPE_UINT32) {
		return uint32Value;
	}

	if (type == VALUE_TYPE_INT64) {
		return (double)int64Value;
	}
	if (type == VALUE_TYPE_UINT64) {
		return (double)uint64Value;
	}

	if (type == VALUE_TYPE_DATE) {
		return doubleValue;
	}

	if (isString()) {
        const char *pStart = getString();
        char *pEnd;
		double value = strtod(pStart, &pEnd);
        while (isspace(*pEnd)) {
            pEnd++;
        }
        if (*pEnd == '\0') {
            return value;
        }
	}

    if (err) {
        *err = 1;
    }
	return NAN;
}

float Value::toFloat(int *err) const {
	if (isIndirectValueType()) {
		return getValue().toFloat(err);
	}

	if (err) {
		*err = 0;
	}

	if (type == VALUE_TYPE_DOUBLE) {
		return (float)doubleValue;
	}

	if (type == VALUE_TYPE_FLOAT) {
		return floatValue;
	}

	if (type == VALUE_TYPE_INT8) {
		return int8Value;
	}
	if (type == VALUE_TYPE_UINT8) {
		return uint8Value;
	}

	if (type == VALUE_TYPE_INT16) {
		return int16Value;
	}
	if (type == VALUE_TYPE_UINT16) {
		return uint16Value;
	}

	if (type == VALUE_TYPE_INT32 || type == VALUE_TYPE_BOOLEAN) {
		return (float)int32Value;
	}

	if (type == VALUE_TYPE_UINT32) {
		return (float)uint32Value;
	}

	if (type == VALUE_TYPE_INT64) {
		return (float)int64Value;
	}
	if (type == VALUE_TYPE_UINT64) {
		return (float)uint64Value;
	}

	if (isString()) {
        const char *pStart = getString();
        char *pEnd;
		float value = strtof(pStart, &pEnd);
        while (isspace(*pEnd)) {
            pEnd++;
        }
        if (*pEnd == '\0') {
            return value;
        }
	}

    if (err) {
        *err = 1;
    }
    return NAN;
}

int32_t Value::toInt32(int *err) const {
	if (isIndirectValueType()) {
		return getValue().toInt32(err);
	}

	if (err) {
		*err = 0;
	}

	if (type == VALUE_TYPE_INT32 || type == VALUE_TYPE_BOOLEAN) {
		return int32Value;
	}

	if (type == VALUE_TYPE_UINT32) {
		return (int32_t)uint32Value;
	}

	if (type == VALUE_TYPE_INT8) {
		return int8Value;
	}
	if (type == VALUE_TYPE_UINT8) {
		return uint8Value;
	}

	if (type == VALUE_TYPE_INT16) {
		return int16Value;
	}
	if (type == VALUE_TYPE_UINT16) {
		return uint16Value;
	}

	if (type == VALUE_TYPE_INT64) {
		return (int32_t)int64Value;
	}
	if (type == VALUE_TYPE_UINT64) {
		return (int32_t)uint64Value;
	}

	if (type == VALUE_TYPE_VALUE_PTR) {
		return pValueValue->toInt32(err);
	}

	if (type == VALUE_TYPE_DOUBLE) {
		return (int32_t)doubleValue;
	}

	if (type == VALUE_TYPE_FLOAT) {
		return (int32_t)floatValue;
	}

	if (isString()) {
        const char *pStart = getString();
        char *pEnd;
		int value = strtol(pStart, &pEnd, 10);
        while (isspace(*pEnd)) {
            pEnd++;
        }
        if (*pEnd == '\0') {
            return value;
        }
	}

    if (err) {
        *err = 1;
    }
	return 0;
}

int64_t Value::toInt64(int *err) const {
	if (isIndirectValueType()) {
		return getValue().toInt64(err);
	}

	if (err) {
		*err = 0;
	}

	if (type == VALUE_TYPE_DOUBLE) {
		return (int64_t)doubleValue;
	}
	if (type == VALUE_TYPE_FLOAT) {
		return (int64_t)floatValue;
	}

	if (type == VALUE_TYPE_INT8) {
		return int8Value;
	}
	if (type == VALUE_TYPE_UINT8) {
		return uint8Value;
	}

	if (type == VALUE_TYPE_INT16) {
		return int16Value;
	}
	if (type == VALUE_TYPE_UINT16) {
		return uint16Value;
	}

	if (type == VALUE_TYPE_INT32 || type == VALUE_TYPE_BOOLEAN) {
		return int32Value;
	}
	if (type == VALUE_TYPE_UINT32) {
		return uint32Value;
	}

	if (type == VALUE_TYPE_INT64) {
		return int64Value;
	}
	if (type == VALUE_TYPE_UINT64) {
		return (int64_t)uint64Value;
	}

	if (isString()) {
        const char *pStart = getString();
        char *pEnd;
		int64_t value = strtol(pStart, &pEnd, 10);
        while (isspace(*pEnd)) {
            pEnd++;
        }
        if (*pEnd == '\0') {
            return value;
        }
	}

    if (err) {
        *err = 1;
    }
	return 0;
}

bool Value::toBool(int *err) const {
	if (isIndirectValueType()) {
		return getValue().toBool(err);
	}

    if (err) {
		*err = 0;
	}

	if (type == VALUE_TYPE_UNDEFINED || type == VALUE_TYPE_NULL) {
		return false;
	}

	if (type == VALUE_TYPE_DOUBLE) {
		return doubleValue != 0;
	}
	if (type == VALUE_TYPE_FLOAT) {
		return floatValue != 0;
	}

	if (type == VALUE_TYPE_INT8) {
		return int8Value != 0;
	}
	if (type == VALUE_TYPE_UINT8) {
		return uint8Value != 0;
	}

	if (type == VALUE_TYPE_INT16) {
		return int16Value != 0;
	}
	if (type == VALUE_TYPE_UINT16) {
		return uint16Value != 0;
	}

	if (type == VALUE_TYPE_INT32 || type == VALUE_TYPE_BOOLEAN) {
		return int32Value != 0;
	}
	if (type == VALUE_TYPE_UINT32) {
		return uint32Value != 0;
	}

	if (type == VALUE_TYPE_INT64) {
		return int64Value != 0;
	}
	if (type == VALUE_TYPE_UINT64) {
		return uint64Value != 0;
	}

	if (type == VALUE_TYPE_DATE) {
		return doubleValue != 0;
	}

	if (isString()) {
		const char *str = getString();
		return str && *str;
	}

	if (isBlob()) {
		auto blobRef = getBlob();
		return blobRef->len > 0;
	}

	if (isArray()) {
		auto array = getArray();
        return array->arraySize != 0;
	}

    if (isJson()) {
        return int32Value != 0;
    }

    if (isWidget()) {
        return pVoidValue != 0;
    }


    if (err) {
        *err = 1;
    }

	return false;
}

Value Value::toString(uint32_t id) const {
	if (isIndirectValueType()) {
		return getValue().toString(id);
	}

	if (isString()) {
		return *this;
	}

    char tempStr[64];

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4474)
#endif

    if (type == VALUE_TYPE_DOUBLE) {
        snprintf(tempStr, sizeof(tempStr), "%g", doubleValue);
    } else if (type == VALUE_TYPE_FLOAT) {
        snprintf(tempStr, sizeof(tempStr), "%g", floatValue);
    } else if (type == VALUE_TYPE_INT8) {
        snprintf(tempStr, sizeof(tempStr), "%" PRId8 "", int8Value);
    } else if (type == VALUE_TYPE_UINT8) {
        snprintf(tempStr, sizeof(tempStr), "%" PRIu8 "", uint8Value);
    } else if (type == VALUE_TYPE_INT16) {
        snprintf(tempStr, sizeof(tempStr), "%" PRId16 "", int16Value);
    } else if (type == VALUE_TYPE_UINT16) {
        snprintf(tempStr, sizeof(tempStr), "%" PRIu16 "", uint16Value);
    } else if (type == VALUE_TYPE_INT32) {
        snprintf(tempStr, sizeof(tempStr), "%" PRId32 "", int32Value);
    } else if (type == VALUE_TYPE_UINT32) {
        snprintf(tempStr, sizeof(tempStr), "%" PRIu32 "", uint32Value);
    } else if (type == VALUE_TYPE_INT64) {
#ifdef PRId64
        snprintf(tempStr, sizeof(tempStr), "%" PRId64 "", int64Value);
#else
        snprintf(tempStr, sizeof(tempStr), "%" PRId32 "", (int32_t)int64Value);
#endif
    } else if (type == VALUE_TYPE_UINT64) {
#ifdef PRIu64
        snprintf(tempStr, sizeof(tempStr), "%" PRIu64 "", uint64Value);
#else
        snprintf(tempStr, sizeof(tempStr), "%" PRIu32 "", (uint32_t)uint64Value);
#endif
    } else {
        toText(tempStr, sizeof(tempStr));
    }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

	return makeStringRef(tempStr, strlen(tempStr), id);
}

Value Value::makeStringRef(const char *str, int len, uint32_t id) {
    auto stringRef = ObjectAllocator<StringRef>::allocate(id);
	if (stringRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

	if (len == -1) {
		len = strlen(str);
	}

    stringRef->str = (char *)alloc(len + 1, id + 1);
    if (stringRef->str == nullptr) {
        ObjectAllocator<StringRef>::deallocate(stringRef);
        return Value(0, VALUE_TYPE_NULL);
    }

    stringCopyLength(stringRef->str, len + 1, str, len);
	stringRef->str[len] = 0;

    stringRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_STRING_REF;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = stringRef;

	return value;
}

Value Value::concatenateString(const Value &str1, const Value &str2) {
    auto stringRef = ObjectAllocator<StringRef>::allocate(0xbab14c6a);;
	if (stringRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

    auto newStrLen = strlen(str1.getString()) + strlen(str2.getString()) + 1;
    stringRef->str = (char *)alloc(newStrLen, 0xb5320162);
    if (stringRef->str == nullptr) {
        ObjectAllocator<StringRef>::deallocate(stringRef);
        return Value(0, VALUE_TYPE_NULL);
    }

    stringCopy(stringRef->str, newStrLen, str1.getString());
    stringAppendString(stringRef->str, newStrLen, str2.getString());

    stringRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_STRING_REF;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = stringRef;

	return value;
}

Value Value::makeArrayRef(int arraySize, int arrayType, uint32_t id) {
    auto ptr = alloc(sizeof(ArrayValueRef) + (arraySize > 0 ? arraySize - 1 : 0) * sizeof(Value), id);
	if (ptr == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

    ArrayValueRef *arrayRef = new (ptr) ArrayValueRef;
    arrayRef->arrayValue.arraySize = arraySize;
    arrayRef->arrayValue.arrayType = arrayType;
    for (int i = 1; i < arraySize; i++) {
        new (arrayRef->arrayValue.values + i) Value();
    }

    arrayRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_ARRAY_REF;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = arrayRef;

	return value;
}

Value Value::makeArrayElementRef(Value arrayValue, int elementIndex, uint32_t id) {
    auto arrayElementValueRef = ObjectAllocator<ArrayElementValue>::allocate(id);
	if (arrayElementValueRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

    arrayElementValueRef->arrayValue = arrayValue;
    arrayElementValueRef->elementIndex = elementIndex;

    arrayElementValueRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_ARRAY_ELEMENT_VALUE;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = arrayElementValueRef;

	return value;
}

Value Value::makeJsonMemberRef(Value jsonValue, Value propertyName, uint32_t id) {
    auto jsonMemberValueRef = ObjectAllocator<JsonMemberValue>::allocate(id);
	if (jsonMemberValueRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

    jsonMemberValueRef->jsonValue = jsonValue;
    jsonMemberValueRef->propertyName = propertyName;

    jsonMemberValueRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_JSON_MEMBER_VALUE;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = jsonMemberValueRef;

	return value;
}

Value Value::makeBlobRef(const uint8_t *blob, uint32_t len, uint32_t id) {
    auto blobRef = ObjectAllocator<BlobRef>::allocate(id);
	if (blobRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

	blobRef->blob = (uint8_t *)alloc(len, id + 1);
    if (blobRef->blob == nullptr) {
        ObjectAllocator<BlobRef>::deallocate(blobRef);
        return Value(0, VALUE_TYPE_NULL);
    }
    blobRef->len = len;

    if (blob) {
        memcpy(blobRef->blob, blob, len);
    } else {
        memset(blobRef->blob, 0, len);
    }

    blobRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_BLOB_REF;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = blobRef;

	return value;
}

Value Value::makeBlobRef(const uint8_t *blob1, uint32_t len1, const uint8_t *blob2, uint32_t len2, uint32_t id) {
    auto blobRef = ObjectAllocator<BlobRef>::allocate(id);
	if (blobRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

	blobRef->blob = (uint8_t *)alloc(len1 + len2, id + 1);
    if (blobRef->blob == nullptr) {
        ObjectAllocator<BlobRef>::deallocate(blobRef);
        return Value(0, VALUE_TYPE_NULL);
    }
    blobRef->len = len1 + len2;

    memcpy(blobRef->blob, blob1, len1);
    memcpy(blobRef->blob + len1, blob2, len2);

    blobRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_BLOB_REF;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = blobRef;

	return value;
}

#if defined(EEZ_FOR_LVGL)
Value Value::makeLVGLEventRef(uint32_t code, void *currentTarget, void *target, int32_t userData, uint32_t key, int32_t gestureDir, int32_t rotaryDiff, uint32_t id) {
    auto lvglEventRef = ObjectAllocator<LVGLEventRef>::allocate(id);
	if (lvglEventRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

	lvglEventRef->code = code;
    lvglEventRef->currentTarget = currentTarget;
    lvglEventRef->target = target;
    lvglEventRef->userData = userData;
    lvglEventRef->key = key;
    lvglEventRef->gestureDir = gestureDir;
    lvglEventRef->rotaryDiff = rotaryDiff;

    lvglEventRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_EVENT;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = lvglEventRef;

	return value;
}
#endif

Value Value::makePropertyRef(flow::FlowState *flowState, int componentIndex, int propertyIndex, uint32_t id) {
    auto propertyRef = ObjectAllocator<PropertyRef>::allocate(id);
	if (propertyRef == nullptr) {
		return Value(0, VALUE_TYPE_NULL);
	}

	propertyRef->flowState = flowState;
    propertyRef->componentIndex = componentIndex;
    propertyRef->propertyIndex = propertyIndex;

    propertyRef->refCounter = 1;

    Value value;

    value.type = VALUE_TYPE_PROPERTY_REF;
    value.options = VALUE_OPTIONS_REF;
    value.refValue = propertyRef;

	return value;
}

Value Value::evalProperty() const {
    auto propertyRef = getPropertyRef();
    Value value;
    flow::evalProperty(propertyRef->flowState, propertyRef->componentIndex, propertyRef->propertyIndex, value, flow::FlowError::Plain("Failed to evaluate an user property in UserWidget"));
    return value;
}

Value Value::clone() {
    if (isArray()) {
        auto array = getArray();
        auto resultArrayValue = makeArrayRef(array->arraySize, array->arrayType, 0x0ea48dcb);
        auto resultArray = resultArrayValue.getArray();

        for (uint32_t elementIndex = 0; elementIndex < array->arraySize; elementIndex++) {
            auto elementValue = array->values[elementIndex].clone();
            if (elementValue.isError()) {
                return elementValue;
            }
            resultArray->values[elementIndex] = elementValue;
        }

        return resultArrayValue;
    } else if (isString()) {
        return makeStringRef(getString(), -1, 0x91846ff3);
    }

    return *this;
}

#if defined(EEZ_OPTION_GUI)

#if !EEZ_OPTION_GUI

Value getVar(int16_t id) {
    auto native_var = native_vars[id];

    if (native_var.type == NATIVE_VAR_TYPE_INTEGER) {
        auto get = (int32_t (*)())native_var.get;
        return Value((int)get(), VALUE_TYPE_INT32);
    }

    if (native_var.type == NATIVE_VAR_TYPE_BOOLEAN) {
        auto get = (bool (*)())native_var.get;
        return Value(get(), VALUE_TYPE_BOOLEAN);
    }

    if (native_var.type == NATIVE_VAR_TYPE_FLOAT) {
        auto get = (float (*)())native_var.get;
        return Value(get(), VALUE_TYPE_FLOAT);
    }

    if (native_var.type == NATIVE_VAR_TYPE_DOUBLE) {
        auto get = (double (*)())native_var.get;
        return Value(get(), VALUE_TYPE_DOUBLE);
    }

    if (native_var.type == NATIVE_VAR_TYPE_STRING) {
        auto get = (const char *(*)())native_var.get;
        return Value(get(), VALUE_TYPE_STRING);
    }

    return Value();
}

void setVar(int16_t id, const Value& value) {
    auto native_var = native_vars[id];

    if (native_var.type == NATIVE_VAR_TYPE_INTEGER) {
        auto set = (void (*)(int32_t))native_var.set;
        set(value.toInt32(nullptr));
    }

    if (native_var.type == NATIVE_VAR_TYPE_BOOLEAN) {
        auto set = (void (*)(bool))native_var.set;
        set(value.toBool(nullptr));
    }

    if (native_var.type == NATIVE_VAR_TYPE_FLOAT) {
        auto set = (void (*)(float))native_var.set;
        set(value.toFloat(nullptr));
    }

    if (native_var.type == NATIVE_VAR_TYPE_DOUBLE) {
        auto set = (void (*)(double))native_var.set;
        set(value.toDouble(nullptr));
    }

    if (native_var.type == NATIVE_VAR_TYPE_STRING) {
        auto set = (void (*)(const char *))native_var.set;
        set(value.getString());
    }
}

#endif // !EEZ_OPTION_GUI

#endif // defined(EEZ_OPTION_GUI)

} // namespace eez
