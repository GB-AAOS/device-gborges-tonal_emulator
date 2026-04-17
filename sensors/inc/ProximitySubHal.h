#pragma once

#include <V2_1/SubHal.h>
#include <hidl/Status.h>
#include <thread>

namespace sensors = android::hardware::sensors;
using sensors::V1_0::Result;
using sensors::V1_0::OperationMode;
using sensors::V1_0::RateLevel;
using sensors::V1_0::SharedMemInfo;
using sensors::V2_1::Event;
using sensors::V2_1::SensorInfo;
using sensors::V2_1::implementation::ISensorsSubHal;
using sensors::V2_1::implementation::IHalProxyCallback;
using android::hardware::Return;
using android::hardware::Void;
using android::hardware::hidl_handle;
using android::hardware::hidl_string;
using android::hardware::hidl_vec;
using android::sp;

class ProximitySubHal : public ISensorsSubHal {
public:
    ProximitySubHal();
    virtual ~ProximitySubHal();

    Return<Result> initialize(const sp<IHalProxyCallback>& halProxyCallback) override;
    Return<void> getSensorsList_2_1(getSensorsList_2_1_cb callback) override;
    Return<Result> setOperationMode(OperationMode mode) override;
    Return<Result> activate(int32_t sensorHandle, bool enabled) override;
    Return<Result> batch(int32_t sensorHandle, int64_t samplingPeriodNs, int64_t maxReportLatencyNs) override;
    Return<Result> flush(int32_t sensorHandle) override;
    Return<Result> injectSensorData_2_1(const Event& event) override;
    Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) override;
    const std::string getName() override { return "Proximity-Fake-Sensor-SubHAL"; }

    // Direct Channel (Unsupported)
    Return<void> registerDirectChannel(const SharedMemInfo& mem, registerDirectChannel_cb callback) override;
    Return<Result> unregisterDirectChannel(int32_t channelHandle) override;
    Return<void> configDirectReport(int32_t sensorHandle, int32_t channelHandle, RateLevel rate, configDirectReport_cb callback) override;

private:
void sensorThreadLoop();

    sp<IHalProxyCallback> mCallback;
    SensorInfo mSensorInfo;
    
    std::atomic<bool> mEnabled;
    std::atomic<bool> mStopThread;
    std::thread mThread;
    
    static constexpr int32_t kSensorHandle = 5;
};