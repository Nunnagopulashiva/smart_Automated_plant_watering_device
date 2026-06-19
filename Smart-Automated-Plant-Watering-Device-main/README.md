# Smart Automated Plant Watering Device

A Smart Automated Plant Watering System developed using ESP32, soil moisture sensor, relay module, and water pump. The system automatically waters plants when the soil becomes dry and provides remote monitoring through a web dashboard.

## Features

- Real-time soil moisture monitoring
- Automatic watering using relay and water pump
- Time-slot based watering (6:00 AM and 5:00 PM)
- ESP32 with Wi-Fi connectivity
- IoT-based web dashboard
- Displays:
  - Soil Moisture Percentage
  - Pump Status
  - Current Time
  - Last Watering Time
  - Next Watering Time
  - Moisture Graph

## Components Used

- ESP32 Development Board
- Capacitive Soil Moisture Sensor
- 1-Channel Relay Module
- DC Water Pump
- 9V Battery / External Power Supply
- Jumper Wires
- Breadboard

## Project Structure

```text
Smart-Automated-Plant-Watering-Device/
│
├── arduino/
│   └── grow.ino
├── app2.py
├── Screenshots/
├── teamplates/
│   └── index.html
├── README.md
├── LICENSE
└── .gitignore
```

The `arduino/` directory contains the ESP32 sketch, and `Screenshots/` contains project and dashboard images.

## Circuit Connections

![ESP32 Smart Automated Plant Watering Device Workflow](Screenshots/Workflow.png)

### Soil Moisture Sensor → ESP32
| Sensor Pin | ESP32 Pin |
|------------|-----------|
| VCC        | 3.3V      |
| GND        | GND       |
| AOUT       | GPIO 34   |

### Relay Module → ESP32
| Relay Pin | ESP32 Pin |
|-----------|-----------|
| VCC       | 5V        |
| GND       | GND       |
| IN        | GPIO 26   |

### Pump and Battery Connection
Battery (+) → Relay COM  
Relay NO    → Pump (+)  
Pump (-)    → Battery (-)

## Working

1. ESP32 reads the soil moisture value.
2. If soil is dry and the time slot is active:
   - 06:00 AM to 06:09 AM
   - 05:00 PM to 05:09 PM
3. Relay turns ON the water pump.
4. ESP32 sends moisture and pump status to Flask server.
5. Web dashboard updates every 2 seconds.

## Installation

1. **Install Python Libraries**
   ```
   pip install flask
   ```

2. **Run Flask Server**
   ```
   python app2.py
   ```

3. **Open Browser**
   ```
   http://YOUR_LOCAL_IP:5000
   ```
   Example:
   ```
   http://192.168.1.5:5000
   ```

4. **Upload ESP32 Code**
   Upload the Arduino code to ESP32 using Arduino IDE.

## Sample Output

```
Moisture: 1712
Current Time: 20:27
Outside Watering Time
Soil Wet - Pump OFF
```

## Results

- Maintains soil moisture between 40% and 70%
- Saves approximately 30–40% water
- Provides fully automatic irrigation
- Reduces manual effort
