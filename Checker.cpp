#include <stdio.h>
#include <stdbool.h>

// Enum to represent status of each check
typedef enum {
    STATUS_OK,
    STATUS_TOO_LOW,
    STATUS_TOO_HIGH
} Status;

const char* statusToString(Status status) {
    switch(status) {
        case STATUS_OK: return "OK";
        case STATUS_TOO_LOW: return "too low";
        case STATUS_TOO_HIGH: return "too high";
        default: return "unknown status";
    }
}

typedef struct {
    const char* parameterName;
    Status status;
} ParameterStatus;

ParameterStatus checkInRange(float value, float min, float max, const char* parameterName) {
    ParameterStatus result = { parameterName, STATUS_OK };
    result.status = (value < min) ? STATUS_TOO_LOW :
                    (value > max) ? STATUS_TOO_HIGH :
                    STATUS_OK;
    if (result.status != STATUS_OK) {
        printf("%s %s!\n", parameterName, statusToString(result.status));
    }
    return result;
}

ParameterStatus checkChargeRate(float chargeRate) {
    ParameterStatus result = { "Charge Rate", STATUS_OK };
    if (chargeRate > 0.8) {
        result.status = STATUS_TOO_HIGH;
        printf("Charge Rate too high!\n");
    }
    return result;
}

typedef struct {
    ParameterStatus temperatureStatus;
    ParameterStatus socStatus;
    ParameterStatus chargeRateStatus;
} BatteryCheckResult;

BatteryCheckResult checkBattery(float temperature, float soc, float chargeRate) {
    BatteryCheckResult result;
    result.temperatureStatus = checkInRange(temperature, 0.0, 45.0, "Temperature");
    result.socStatus = checkInRange(soc, 20.0, 80.0, "State of Charge");
    result.chargeRateStatus = checkChargeRate(chargeRate);
    return result;
}

bool batteryIsOk(BatteryCheckResult result) {
    return (result.temperatureStatus.status == STATUS_OK) &&
           (result.socStatus.status == STATUS_OK) &&
           (result.chargeRateStatus.status == STATUS_OK);
}

int main() {
    BatteryCheckResult result;

    result = checkBattery(25.0, 70.0, 0.7);
    printf("Battery OK? %s\n", batteryIsOk(result) ? "Yes" : "No");

    result = checkBattery(-5.0, 70.0, 0.7);
    printf("Battery OK? %s\n", batteryIsOk(result) ? "Yes" : "No");

    result = checkBattery(25.0, 85.0, 0.7);
    printf("Battery OK? %s\n", batteryIsOk(result) ? "Yes" : "No");

    result = checkBattery(25.0, 70.0, 0.9);
    printf("Battery OK? %s\n", batteryIsOk(result) ? "Yes" : "No");

    return 0;
}
