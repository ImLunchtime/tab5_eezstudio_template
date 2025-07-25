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

#include <string.h>

#include <eez/core/unit.h>

#if OPTION_SCPI
#include <scpi/types.h>
#endif

namespace eez {

const char *g_unitNames[] = {
    "", // UNIT_NONE
    "V", // UNIT_VOLT
    "mV", // UNIT_MILLI_VOLT
    "A", // UNIT_AMPER
    "mA", // UNIT_MILLI_AMPER
    "uA", // UNIT_MICRO_AMPER
    "W", // UNIT_WATT
    "mW", // UNIT_MILLI_WATT
    "s", // UNIT_SECOND
    "ms", // UNIT_MILLI_SECOND
    DEGREE_SYMBOL"C", // UNIT_CELSIUS
    "rpm", // UNIT_RPM
    "\xb4", // UNIT_OHM
    "K\xb4", // UNIT_KOHM
    "M\xb4", // UNIT_MOHM
    "%", // UNIT_PERCENT
    "Hz", // UNIT_HERTZ
    "mHz", // UNIT_MILLI_HERTZ
    "KHz", // UNIT_KHERTZ
    "MHz", // UNIT_MHERTZ
    "J", // UNIT_JOULE
    "F", // UNIT_FARAD
    "mF", // UNIT_MILLI_FARAD
    "uF", // UNIT_MICRO_FARAD
    "nF", // UNIT_NANO_FARAD
    "pF", // UNIT_PICO_FARAD
    "minutes", // UNIT_MINUTE
    "VA", // UNIT_VOLT_AMPERE
    "VAR", // UNIT_VOLT_AMPERE_REACTIVE
	DEGREE_SYMBOL, // UNIT_DEGREE
	"Vpp", // UNIT_VOLT_PP
	"mVpp", // UNIT_MILLI_VOLT_PP
	"App", // UNIT_AMPER_PP
	"mApp", // UNIT_MILLI_AMPER_PP
	"uApp", // UNIT_MICRO_AMPER_PP
};

const Unit g_baseUnit[] = {
	UNIT_NONE, // UNIT_NONE
	UNIT_VOLT, // UNIT_VOLT
	UNIT_VOLT, // UNIT_MILLI_VOLT
	UNIT_AMPER, // UNIT_AMPER
	UNIT_AMPER, // UNIT_MILLI_AMPER
	UNIT_AMPER, // UNIT_MICRO_AMPER
	UNIT_WATT, // UNIT_WATT
	UNIT_WATT, // UNIT_MILLI_WATT
	UNIT_SECOND, // UNIT_SECOND
	UNIT_SECOND, // UNIT_MILLI_SECOND
	UNIT_CELSIUS, // UNIT_CELSIUS
	UNIT_RPM, // UNIT_RPM
	UNIT_OHM, // UNIT_OHM
	UNIT_OHM, // UNIT_KOHM
	UNIT_OHM, // UNIT_MOHM
	UNIT_PERCENT, // UNIT_PERCENT
	UNIT_HERTZ, // UNIT_HERTZ
	UNIT_HERTZ, // UNIT_MILLI_HERTZ
	UNIT_HERTZ, // UNIT_KHERTZ
	UNIT_HERTZ, // UNIT_MHERTZ
	UNIT_JOULE, // UNIT_JOULE
	UNIT_FARAD, // UNIT_FARAD
	UNIT_FARAD, // UNIT_MILLI_FARAD
	UNIT_FARAD, // UNIT_MICRO_FARAD
	UNIT_FARAD, // UNIT_NANO_FARAD
	UNIT_FARAD, // UNIT_PICO_FARAD
	UNIT_SECOND, // UNIT_MINUTE
	UNIT_VOLT_AMPERE, // UNIT_VOLT_AMPERE
	UNIT_VOLT_AMPERE, //UNIT_VOLT_AMPERE_REACTIVE
	UNIT_DEGREE, // UNIT_DEGREE
	UNIT_VOLT_PP, // UNIT_VOLT_PP
	UNIT_VOLT_PP, // UNIT_MILLI_VOLT_PP
	UNIT_AMPER_PP, // UNIT_AMPER_PP
	UNIT_AMPER_PP, // UNIT_MILLI_AMPER_PP
	UNIT_AMPER_PP, // UNIT_MICRO_AMPER_PP
};

const float g_unitFactor[] = {
	1.0f, // UNIT_NONE
	1.0f, // UNIT_VOLT
	1E-3f, // UNIT_MILLI_VOLT
	1.0f, // UNIT_AMPER
	1E-3f, // UNIT_MILLI_AMPER
	1E-6f, // UNIT_MICRO_AMPER
	1.0f, // UNIT_WATT
	1E-3f, // UNIT_MILLI_WATT
	1.0f, // UNIT_SECOND
	1E-3f, // UNIT_MILLI_SECOND
	1.0f, // UNIT_CELSIUS
	1.0f, // UNIT_RPM
	1.0f, // UNIT_OHM
	1E3f, // UNIT_KOHM
	1E6f, // UNIT_MOHM
	1.0f, // UNIT_PERCENT
	1.0f, // UNIT_HERTZ
	1E-3f, // UNIT_MILLI_HERTZ
	1E3f, // UNIT_KHERTZ
	1E6f, // UNIT_MHERTZ
	1.0f, // UNIT_JOULE
	1.0f, // UNIT_FARAD
	1E-3f, // UNIT_MILLI_FARAD
	1E-6f, // UNIT_MICRO_FARAD
	1E-9f, // UNIT_NANO_FARAD
	1E-12f, // UNIT_PICO_FARAD
	60.0f, // UNIT_MINUTE
	1.0f, // UNIT_VOLT_AMPERE
	1.0f, //UNIT_VOLT_AMPERE_REACTIVE
	1.0f, // UNIT_DEGREE
	1.0f, // UNIT_VOLT_PP
	1E-3f, // UNIT_MILLI_VOLT_PP
	1.0f, // UNIT_AMPER_PP
	1E-3f, // UNIT_MILLI_AMPER_PP
	1E-6f, // UNIT_MICRO_AMPER_PP
};

#if OPTION_SCPI
static const int g_scpiUnits[] = {
    SCPI_UNIT_NONE, // UNIT_NONE
    SCPI_UNIT_VOLT, // UNIT_VOLT
    SCPI_UNIT_VOLT, // UNIT_MILLI_VOLT
    SCPI_UNIT_AMPER, // UNIT_AMPER
    SCPI_UNIT_AMPER, // UNIT_MILLI_AMPER
    SCPI_UNIT_AMPER, // UNIT_MICRO_AMPER
    SCPI_UNIT_WATT, // UNIT_WATT
    SCPI_UNIT_WATT, // UNIT_MILLI_WATT
    SCPI_UNIT_SECOND, // UNIT_SECOND
    SCPI_UNIT_SECOND, // UNIT_MILLI_SECOND
    SCPI_UNIT_CELSIUS, // UNIT_CELSIUS
    SCPI_UNIT_NONE, // UNIT_RPM
    SCPI_UNIT_OHM, // UNIT_OHM
    SCPI_UNIT_OHM, // UNIT_KOHM
    SCPI_UNIT_OHM, // UNIT_MOHM
    SCPI_UNIT_NONE, // UNIT_PERCENT
    SCPI_UNIT_HERTZ, // UNIT_HERTZ
    SCPI_UNIT_HERTZ, // UNIT_MILLI_HERTZ
    SCPI_UNIT_HERTZ, // UNIT_KHERTZ
    SCPI_UNIT_HERTZ, // UNIT_MHERTZ
    SCPI_UNIT_JOULE, // UNIT_JOULE
    SCPI_UNIT_FARAD, // UNIT_FARAD
    SCPI_UNIT_FARAD, // UNIT_MILLI_FARAD
    SCPI_UNIT_FARAD, // UNIT_MICRO_FARAD
    SCPI_UNIT_FARAD, // UNIT_NANO_FARAD
    SCPI_UNIT_FARAD, // UNIT_PICO_FARAD
	SCPI_UNIT_SECOND, // UNIT_MINUTE
	SCPI_UNIT_WATT, // UNIT_VOLT_AMPERE
	SCPI_UNIT_WATT, // UNIT_VOLT_AMPERE_REACTIVE
	SCPI_UNIT_DEGREE, // UNIT_DEGREE
	SCPI_UNIT_VOLT, // UNIT_VOLT_PP
	SCPI_UNIT_VOLT, // UNIT_MILLI_VOLT_PP
	SCPI_UNIT_AMPER, // UNIT_AMPER_PP
	SCPI_UNIT_AMPER, // UNIT_MILLI_AMPER_PP
	SCPI_UNIT_AMPER, // UNIT_MICRO_AMPER_PP
};
#endif

Unit getUnitFromName(const char *unitName) {
	if (unitName) {
		for (unsigned i = 0; i < sizeof(g_unitNames) / sizeof(const char *); i++) {
			if (strcmp(g_unitNames[i], unitName) == 0) {
				return (Unit)i;
			}
		}
	}
	return UNIT_NONE;
}

#if OPTION_SCPI
int getScpiUnit(Unit unit) {
    if (unit == UNIT_UNKNOWN) {
        return SCPI_UNIT_NONE;
    }
    return g_scpiUnits[unit];
}
#endif

Unit getBaseUnit(Unit unit) {
    if (unit == UNIT_UNKNOWN) {
        return UNIT_UNKNOWN;
    }
	return g_baseUnit[unit];
}

float getUnitFactor(Unit unit) {
    if (unit == UNIT_UNKNOWN) {
        return 1.0f;
    }
	return g_unitFactor[unit];
}

static Unit getDerivedUnit(Unit unit, float factor) {
	if (unit == UNIT_UNKNOWN) {
		return UNIT_UNKNOWN;
	}

	for (size_t i = 0; i < sizeof(g_baseUnit) / sizeof(Unit); i++) {
		if (g_baseUnit[i] == g_baseUnit[unit] && g_unitFactor[i] == factor) {
			return (Unit)i;
		}
	}

	return UNIT_UNKNOWN;
}

static const float FACTORS[] = { 1E-12F, 1E-9F, 1E-6F, 1E-3F, 1E0F, 1E3F, 1E6F, 1E9F, 1E12F };

Unit findDerivedUnit(float value, Unit unit) {
	Unit result;

	for (int factorIndex = 1; ; factorIndex++) {
		float factor = FACTORS[factorIndex];
		if (factor > 1.0F) {
			break;
		}
		if (value < factor) {
			result = getDerivedUnit(unit, FACTORS[factorIndex - 1]);
			if (result != UNIT_UNKNOWN) {
				return result;
			}
		}
	}

	for (int factorIndex = sizeof(FACTORS) / sizeof(float) - 1; factorIndex >= 0; factorIndex--) {
		float factor = FACTORS[factorIndex];
		if (factor == 1.0F) {
			break;
		}
		if (value >= factor) {
			result = getDerivedUnit(unit, factor);
			if (result != UNIT_UNKNOWN) {
				return result;
			}
		}
	}

	return unit;
}

static float getSmallerFactor(float factor) {
	for (int factorIndex = sizeof(FACTORS) / sizeof(float) - 1; factorIndex > 0; factorIndex--) {
		float itFactor = FACTORS[factorIndex];
		if (itFactor < factor) {
			return itFactor;
		}
	}
	return FACTORS[0];
}

Unit getSmallerUnit(Unit unit, float min, float precision) {
	float factor = getUnitFactor(unit);
	if (precision <= factor || min <= factor) {
		return getDerivedUnit(unit, getSmallerFactor(factor));
	}
	return UNIT_UNKNOWN;
}

Unit getBiggestUnit(Unit unit, float max) {
	for (int factorIndex = sizeof(FACTORS) / sizeof(float) - 1; factorIndex >= 0; factorIndex--) {
		float factor = FACTORS[factorIndex];
		if (max >= factor) {
			auto result = getDerivedUnit(unit, factor);
			if (result != UNIT_UNKNOWN) {
				return result;
			}
		}
	}
	return UNIT_UNKNOWN;
}

Unit getSmallestUnit(Unit unit, float min, float precision) {
	for (int factorIndex = 0; factorIndex < int(sizeof(FACTORS) / sizeof(float)); factorIndex++) {
		float factor = FACTORS[factorIndex];
		if (precision <= factor || min <= factor) {
			auto result = getDerivedUnit(unit, factor);
			if (result != UNIT_UNKNOWN) {
				return result;
			}
		}
	}
	return UNIT_UNKNOWN;
}

} // namespace eez
