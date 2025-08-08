#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

// Generic range checker for temperature and SOC
bool isInRange(float value, float min, float max, const char* parameterName) {
    if (value < min || value > max) {
        printf("%s out of range!\n", parameterName);
        return false;
    }
    return true;
}

// Specialized check for chargeRate
bool isChargeRateOk(float chargeRate) {
    // Only upper limit check, no assumption on minimum
    if (chargeRate > 0.8) {
        printf("Charge Rate out of range!\n");
        return false;
    }
    return true;
}

bool batteryIsOk(float temperature, float soc, float chargeRate) {
    bool tempOk = isInRange(temperature, 0.0, 45.0, "Temperature");
    bool socOk = isInRange(soc, 20.0, 80.0, "State of Charge");
    bool chargeRateOk = isChargeRateOk(chargeRate);

    return tempOk && socOk && chargeRateOk;
}

int main() {
    assert(batteryIsOk(25.0, 70.0, 0.7) == true);
    assert(batteryIsOk(50.0, 85.0, 0.0) == false); // allowed if no minimum set for chargeRate
    return 0;
}
