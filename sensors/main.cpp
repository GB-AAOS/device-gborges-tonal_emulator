#include "ProximitySubHal.h"

static ProximitySubHal gProximitySubHal;

extern "C" ISensorsSubHal* sensorsHalGetSubHal_2_1(uint32_t* version) {
    if (version == nullptr) {
        return nullptr;
    }
    *version = SUB_HAL_2_1_VERSION;
    return &gProximitySubHal;
}