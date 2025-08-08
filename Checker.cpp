#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// Enum to represent status of each check
typedef enum {
    STATUS_OK,
    STATUS_TOO_LOW,
    STATUS_TOO_HIGH
} Status;

// Function to convert Status to string (for printing)
const char* statusToString(Status status) {
    switch(status) {
        case STATUS_OK: return "OK";
        case STATUS_TOO_LOW: return "Too Low";
        case STATUS_TOO_HIGH: return "Too High";
        default: return "Unknown";
    }
}

// Struct to hold the check result for a parameter
typedef struct {
    const char* parameterName;
    Status status;
} ParameterStatus;

// Generic checker for temperature and SoC
ParameterStatus checkInRange(float value, float min, float max, const char* parameterName) {
    ParameterStatus result = { parameterName, STATUS_OK };
    if (value < min) {
        result.status = STATUS_TOO_LOW;
        printf("%s too low!\n", parameterName);
    } else if (value > max) {
        result.status = STATUS_TOO_HIGH;
        printf("%s too high!\n", parameterName);
    }
    return result;
}

// Specialized checker for chargeRate
ParameterStatus checkChargeRate(float chargeRate) {
    ParameterStatus result = { "Charge Rate", STATUS_OK };
    // Only upper limit check
    if (chargeRate > 0.8) {
        result.status = STATUS_TOO_HIGH;
        printf("Charge Rate too high!\n");
    }
    // No min assumed - no low check
    return result;
}

// Struct to hold overall battery check results
typedef struct {
    ParameterStatus temperatureStatus;
    ParameterStatus socStatus;
    ParameterStatus chargeRateStatus;
} BatteryCheckResult;

// Perform all checks and return detailed results
BatteryCheckResult checkBattery(float temperature, float soc, float chargeRate) {
    BatteryCheckResult result;
    result.temperatureStatus = checkInRange(temperature, 0.0, 45.0, "Temperature");
    result.socStatus = checkInRange(soc, 20.0, 80.0, "State of Charge");
    result.chargeRateStatus = checkChargeRate(chargeRate);
    return result;
}

// Helper to decide if battery is OK overall
bool batteryIsOk(BatteryCheckResult result) {
    return (result.temperatureStatus.status == STATUS_OK) &&
           (result.socStatus.status == STATUS_OK) &&
           (result.chargeRateStatus.status == STATUS_OK);
}

// TESTS

void runTests() {
    BatteryCheckResult result;

    // Test 1: All OK
    result = checkBattery(25.0, 70.0, 0.7);
    assert(result.temperatureStatus.status == STATUS_OK);
    assert(result.socStatus.status == STATUS_OK);
    assert(result.chargeRateStatus.status == STATUS_OK);
    assert(batteryIsOk(result) == true);

    // Test 2: Temperature too low
    result = checkBattery(-0.1, 50.0, 0.7);
    assert(result.temperatureStatus.status == STATUS_TOO_LOW);
    assert(result.socStatus.status == STATUS_OK);
    assert(result.chargeRateStatus.status == STATUS_OK);
    assert(batteryIsOk(result) == false);

    // Test 3: Temperature too high
    result = checkBattery(46.0, 50.0, 0.7);
    assert(result.temperatureStatus.status == STATUS_TOO_HIGH);
    assert(result.socStatus.status == STATUS_OK);
    assert(result.chargeRateStatus.status == STATUS_OK);
    assert(batteryIsOk(result) == false);

    // Test 4: SoC too low
    result = checkBattery(25.0, 19.9, 0.7);
    assert(result.temperatureStatus.status == STATUS_OK);
    assert(result.socStatus.status == STATUS_TOO_LOW);
    assert(result.chargeRateStatus.status == STATUS_OK);
    assert(batteryIsOk(result) == false);

    // Test 5: SoC too high
    result = checkBattery(25.0, 80.1, 0.7);
    assert(result.temperatureStatus.status == STATUS_OK);
    assert(result.socStatus.status == STATUS_TOO_HIGH);
    assert(result.chargeRateStatus.status == STATUS_OK);
    assert(batteryIsOk(result) == false);

    // Test 6: Charge rate too high
    result = checkBattery(25.0, 50.0, 0.81);
    assert(result.temperatureStatus.status == STATUS_OK);
    assert(result.socStatus.status == STATUS_OK);
    assert(result.chargeRateStatus.status == STATUS_TOO_HIGH);
    assert(batteryIsOk(result) == false);

    // Test 7: Charge rate negative (allowed)
    result = checkBattery(25.0, 50.0, -0.5);
    assert(result.temperatureStatus.status == STATUS_OK);
    assert(result.socStatus.status == STATUS_OK);
    assert(result.chargeRateStatus.status == STATUS_OK);
    assert(batteryIsOk(result) == true);

    // Test 8: Multiple failures
    result = checkBattery(-10.0, 10.0, 1.0);
    assert(result.temperatureStatus.status == STATUS_TOO_LOW);
    assert(result.socStatus.status == STATUS_TOO_LOW);
    assert(result.chargeRateStatus.status == STATUS_TOO_HIGH);
    assert(batteryIsOk(result) == false);

    printf("All tests passed!\n");
}

int main() {
    runTests();
    return 0;
}
