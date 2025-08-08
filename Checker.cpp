#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

// Individual checks
bool isTemperatureOk(float temperature) {
    if (temperature < 0 || temperature > 45) {
        printf("Temperature out of range!\n");
        return false;
    }
    return true;
}

bool isSocOk(float soc) {
    if (soc < 20 || soc > 80) {
        printf("State of Charge out of range!\n");
        return false;
    }
    return true;
}

bool isChargeRateOk(float chargeRate) {
    if (chargeRate > 0.8) {
        printf("Charge Rate out of range!\n");
        return false;
    }
    return true;
}

// Composed check
bool batteryIsOk(float temperature, float soc, float chargeRate) {
    bool tempOk = isTemperatureOk(temperature);
    bool socOk = isSocOk(soc);
    bool chargeRateOk = isChargeRateOk(chargeRate);

    return tempOk && socOk && chargeRateOk;
}

int main() {
    assert(batteryIsOk(25, 70, 0.7) == true);
    assert(batteryIsOk(50, 85, 0) == false);
    return 0;
}
