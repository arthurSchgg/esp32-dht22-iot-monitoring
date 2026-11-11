ESP32 DHT22 — IoT Temperature & Humidity Monitoring
A simulated Internet of Things (IoT) project that collects temperature and humidity data from a DHT22 sensor connected to an ESP32, publishes the readings over MQTT, processes them with Node-RED, and monitors them on WEGnology.
Technologies
- ESP32 — microcontroller running the sensor-reading and MQTT publishing code.
- DHT22 — temperature and humidity sensor.
- Wokwi — ESP32 and DHT22 circuit simulation.
- C++ / Arduino — ESP32 firmware.
- MQTT / HiveMQ — communication between the ESP32 and Node-RED.
- Node-RED — data processing, dashboards, and forwarding to WEGnology.
- WEGnology — IoT device monitoring, visualization, and historical data.
System Architecture
DHT22 Sensor
    |
    v
ESP32 (Wokwi)
    | Publishes temperature and humidity via MQTT
    v
HiveMQ Public Broker
    | Node-RED subscribes to both topics
    v
Node-RED
    |-- Local gauges and charts
    |
    `-- Formats JSON and publishes via authenticated MQTT
            |
            v
       WEGnology
       Device attributes, dashboards and history
Project Structure
esp32-dht22-iot-monitoring/
├── wokwi/
│   ├── sketch.ino
│   ├── diagram.json
│   └── libraries.txt
├── node-red/
│   └── flow.json
├── docs/
│   ├── circuito.png
│   ├── node-red.png
│   └── wegnology.png
└── README.md
Add the corresponding files to these folders. The screenshots will appear below once they are uploaded.

Wokwi Circuit
The ESP32 reads the DHT22 sensor connected to GPIO 4. The firmware connects to the simulated Wokwi-GUEST Wi-Fi network and publishes new readings approximately every 2 seconds.

MQTT Topics
Topic	Content	Example
temp_wokwi_dht_pv	Temperature in °C	25.50
hum_wokwi_dht_pv	Relative humidity in %	65.2


Simulation broker: broker.hivemq.com on port 1883.
Node-RED Flow
Node-RED subscribes to both MQTT topics and:
1. Displays the incoming readings using debug nodes.
2. Shows temperature and humidity in gauges.
3. Plots the measurements in charts.
4. Combines both readings in a function node and forwards them to WEGnology through an MQTT out node.

WEGnology Integration
A device named ESP32_DHT22_Wokwi is registered in WEGnology with two Number attributes:
Attribute	Measurement
temperatura	Temperature (°C)
umidade	Relative humidity (%)


Node-RED publishes device state to the WEGnology MQTT broker using the topic:
wnology/<DEVICE_ID>/state
Example payload:
{
  "data": {
    "temperatura": 25.5,
    "umidade": 65.2
  }
}
The WEGnology dashboard displays the measurements with gauges and time-series charts.

How to Run
1. Start the Wokwi simulation. Open the files in wokwi/, ensure the DHTesp and PubSubClient libraries are available, and run the ESP32 simulation.
2. Import the Node-RED flow. In Node-RED, choose Menu → Import and select node-red/flow.json. Configure the HiveMQ connection and verify the two subscription topics.
3. Configure WEGnology. Create a device with the temperatura and umidade attributes. Generate an access key with permission to publish this device's state.
4. Connect Node-RED to WEGnology. Configure its MQTT output with the WEGnology broker (broker.app.wnology.io, port 8883, TLS enabled), the appropriate credentials, and the device state topic. Replace <DEVICE_ID> with your device ID.
5. Deploy and test. Deploy the Node-RED flow. Check the debug nodes, gauges, and charts, then confirm that readings appear in the WEGnology dashboard.
Security Notes
- Never commit WEGnology access keys, access secrets, passwords, or other credentials.
- Replace sensitive configuration values with placeholders before exporting and publishing your Node-RED flow.
- The public HiveMQ broker used in this demonstration is not appropriate for confidential or production data. Prefer authenticated, encrypted connections for real deployments.
- Use unique MQTT client IDs and topic names when reproducing the project to avoid conflicts with other public-broker users.
Results
The system demonstrates a complete IoT data pipeline: sensor simulation → MQTT publication → Node-RED processing → cloud monitoring. Temperature and humidity can be viewed locally in Node-RED and remotely in WEGnology.
Purpose
This educational project demonstrates ESP32 programming, sensor integration, MQTT communication, flow-based processing, and IoT dashboards.