# app.py

from flask import Flask, request, jsonify, render_template
from datetime import datetime

app = Flask(__name__)

moisture_value = 0
pump_status = "OFF"
last_watering = "--"

@app.route("/update")
def update():
    global moisture_value, pump_status, last_watering

    value = request.args.get("value")
    pump = request.args.get("pump")

    if value is not None:
        moisture_value = int(value)

    if pump is not None:
        pump_status = pump

    if pump_status == "ON":
        last_watering = datetime.now().strftime("%I:%M:%S %p")

    return "OK"


@app.route("/data")
def data():

    # Convert raw sensor value to percentage
    moisture_percent = int((4095 - moisture_value) / 4095 * 100)
    moisture_percent = max(0, min(100, moisture_percent))

    # Calculate next watering time
    now = datetime.now()
    hour = now.hour

    if hour < 6:
        next_watering = "06:00 AM"
    elif hour < 17:
        next_watering = "05:00 PM"
    else:
        next_watering = "Tomorrow 06:00 AM"

    return jsonify({
        "moisture": moisture_percent,
        "pump": pump_status,
        "last": last_watering,
        "next": next_watering,
        "current_time": now.strftime("%I:%M:%S %p")
    })


@app.route("/")
def home():
    return render_template("index.html")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)