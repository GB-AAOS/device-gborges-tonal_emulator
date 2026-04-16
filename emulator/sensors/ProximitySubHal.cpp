#include "ProximitySubHal.h"

ProximitySubHal::ProximitySubHal() {
    mSensorInfo.sensorHandle = kSensorHandle;
    mSensorInfo.name = "Proximity Fake Sensor";
    mSensorInfo.vendor = "GBorges";
    mSensorInfo.version = 1;
    mSensorInfo.type = sensors::V2_1::SensorType::PROXIMITY;
    mSensorInfo.typeAsString = "";
    mSensorInfo.maxRange = 1.0f;
    mSensorInfo.resolution = 1.0f;
    mSensorInfo.power = 0.05f;
    mSensorInfo.minDelay = 500000;   // 500ms
    mSensorInfo.fifoReservedEventCount = 0;
    mSensorInfo.fifoMaxEventCount = 0;
    mSensorInfo.requiredPermission = "";
    mSensorInfo.flags = (uint32_t)sensors::V1_0::SensorFlagBits::CONTINUOUS_MODE;
}

Return<void> ProximitySubHal::getSensorsList_2_1(getSensorsList_2_1_cb callback) {
    std::vector<SensorInfo> sensors;
    sensors.push_back(mSensorInfo);
    callback(sensors);
    return Void();
}

Return<Result> ProximitySubHal::activate(int32_t, bool) { return Result::OK; }
Return<Result> ProximitySubHal::setOperationMode(OperationMode) { return Result::OK; }
Return<Result> ProximitySubHal::batch(int32_t, int64_t, int64_t) { return Result::OK; }
Return<Result> ProximitySubHal::flush(int32_t) { return Result::OK; }
Return<Result> ProximitySubHal::injectSensorData_2_1(const Event&) { return Result::INVALID_OPERATION; }
Return<void> ProximitySubHal::debug(const hidl_handle&, const hidl_vec<hidl_string>&) { return Void(); }