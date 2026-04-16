#include <android/sensor.h>
#include <android-base/parseint.h>
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <utils/SystemClock.h>

using namespace std;

void PrintUsage(const char* prog) {
    cout << "Usage: " << prog << " [-o|--once]" << endl;
    cout << "  -o, --once    Read one value and exit" << endl;
}

int main(int argc, char** argv) {
    bool once = false;

    static struct option long_options[] = {
        {"once", no_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int c;
    while ((c = getopt_long(argc, argv, "o", long_options, nullptr)) != -1) {
        if (c == 'o') once = true;
        else { PrintUsage(argv[0]); return 1; }
    }

    // 1. Initialize Sensor Manager
    // Using a package name helps with permission tracking in the SensorService
    ASensorManager* sensorManager = ASensorManager_getInstanceForPackage("gborges.proximity.cli");
    if (!sensorManager) {
        cerr << "Error: Could not get ASensorManager" << endl;
        return 1;
    }

    // 2. Find the Proximity Sensor
    const ASensor* proximitySensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_PROXIMITY);
    if (!proximitySensor) {
        cerr << "Error: Proximity sensor not found!" << endl;
        return 1;
    }

    cout << "Target: " << ASensor_getName(proximitySensor) << " by " << ASensor_getVendor(proximitySensor) << endl;
    cout << left << setw(10) << "Value" << setw(22) << "Timestamp (ns)" << "Latency (ms)" << endl;
    cout << string(50, '-') << endl;

    // 3. Setup the Looper and Event Queue
    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ASensorEventQueue* queue = ASensorManager_createEventQueue(sensorManager, looper, 0, nullptr, nullptr);

    ASensorEventQueue_enableSensor(queue, proximitySensor);
    // Set to 500ms (500,000 microseconds) to match our HAL output frequency
    ASensorEventQueue_setEventRate(queue, proximitySensor, 500000); 

    ASensorEvent event;
    while (true) {
        // Poll the looper. 1000ms timeout.
        if (ALooper_pollAll(1000, nullptr, nullptr, nullptr) == 0) {
            while (ASensorEventQueue_getEvents(queue, &event, 1) > 0) {
                int64_t arrivalTime = android::elapsedRealtimeNano();
                
                // Latency = Arrival Time - Event Generation Timestamp
                double latencyMs = (arrivalTime - event.timestamp) / 1000000.0;

                cout << left << setw(10) << fixed << setprecision(3) << event.distance
                     << setw(22) << event.timestamp
                     << fixed << setprecision(2) << latencyMs << " ms" << endl;

                if (once) goto exit_label;
            }
        }
    }

exit_label:
    ASensorEventQueue_disableSensor(queue, proximitySensor);
    ASensorManager_destroyEventQueue(sensorManager, queue);
    return 0;
}