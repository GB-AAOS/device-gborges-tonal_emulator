package com.gborges.proximity_monitor

import android.app.Activity
import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.os.Bundle
import android.os.SystemClock
import android.widget.TextView

class MainActivity : Activity(), SensorEventListener {

    private lateinit var sensorManager: SensorManager
    private var proximitySensor: Sensor? = null
    
    private lateinit var nameText: TextView
    private lateinit var valueText: TextView
    private lateinit var latencyText: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        nameText = findViewById(R.id.sensor_name)
        valueText = findViewById(R.id.sensor_value)
        latencyText = findViewById(R.id.sensor_latency)

        sensorManager = getSystemService(Context.SENSOR_SERVICE) as SensorManager
        proximitySensor = sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY)

        proximitySensor?.let {
            nameText.text = "${it.name}\n${it.vendor}"
        } ?: run {
            nameText.text = "Error: Proximity Sensor Not Found"
        }
    }

    override fun onResume() {
        super.onResume()
        proximitySensor?.let {
            sensorManager.registerListener(this, it, SensorManager.SENSOR_DELAY_FASTEST)
        }
    }

    override fun onPause() {
        super.onPause()
        sensorManager.unregisterListener(this)
    }

    override fun onSensorChanged(event: SensorEvent) {
        val value = event.values[0]
        val timestampNs = event.timestamp
        val currentNs = SystemClock.elapsedRealtimeNanos()
        
        val latencyMs = (currentNs - timestampNs) / 1_000_000.0

        valueText.text = String.format("%.3f", value)
        latencyText.text = String.format("Latency: %.2f ms", latencyMs)
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {}
}