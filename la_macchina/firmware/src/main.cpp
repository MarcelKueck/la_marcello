#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "config.h"

enum State
{
    IDLE,
    BREWING,
    STOPPED
};

State current_state = IDLE;
bool switch_pressed = false;
unsigned long start_time = 0;
unsigned long timer_duration_millis = DEFAULT_TIMER_SECONDS * 1000;

WebServer server(WEB_SERVER_PORT);

// Handler for status API (returns JSON)
void handleStatus()
{
    String json = "{";
    json += "\"state\":\"";

    switch (current_state)
    {
    case IDLE:
        json += "IDLE";
        break;
    case BREWING:
        json += "BREWING";
        break;
    case STOPPED:
        json += "STOPPED";
        break;
    }

    json += "\",";
    json += "\"timerDuration\":";
    json += timer_duration_millis / 1000;
    json += ",";
    json += "\"elapsed\":";

    if (current_state == BREWING)
    {
        unsigned long elapsed = (millis() - start_time) / 1000;
        json += elapsed;
    }
    else
    {
        json += "0";
    }

    json += ",";
    json += "\"remaining\":";

    if (current_state == BREWING)
    {
        unsigned long elapsed = millis() - start_time;
        if (elapsed < timer_duration_millis)
        {
            json += (timer_duration_millis - elapsed) / 1000;
        }
        else
        {
            json += "0";
        }
    }
    else
    {
        json += timer_duration_millis / 1000;
    }

    json += "}";

    server.send(200, "application/json", json);
}

// Handler for root page (/)
void handleRoot()
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>La Marcello Shot Timer</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            background: linear-gradient(135deg, #1a1a1a 0%, #2d2d2d 100%);
            color: #e0e0e0;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            max-width: 500px;
            width: 100%;
            background: rgba(40, 40, 40, 0.95);
            border-radius: 24px;
            padding: 32px 24px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.4);
            backdrop-filter: blur(10px);
        }
        
        h1 {
            text-align: center;
            margin-bottom: 8px;
            font-size: 2em;
            font-weight: 300;
            letter-spacing: 0.5px;
        }
        
        .brand {
            color: #D32F2F;
            font-family: 'Georgia', serif;
            font-weight: 700;
            font-style: italic;
            letter-spacing: 1px;
            text-shadow: 0 2px 8px rgba(211, 47, 47, 0.3);
        }
        
        .subtitle {
            text-align: center;
            color: #888;
            font-size: 0.9em;
            margin-bottom: 32px;
            font-weight: 300;
        }
        
        .status-card {
            background: linear-gradient(135deg, #2d2d2d 0%, #3a3a3a 100%);
            border-radius: 16px;
            padding: 24px;
            margin-bottom: 24px;
            text-align: center;
            border: 1px solid rgba(255, 255, 255, 0.05);
        }
        
        .status-label {
            color: #888;
            font-size: 0.85em;
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-bottom: 8px;
        }
        
        .status-value {
            font-size: 1.1em;
            font-weight: 600;
            padding: 8px 16px;
            border-radius: 8px;
            display: inline-block;
            margin-bottom: 4px;
        }
        
        .status-idle {
            background: rgba(76, 175, 80, 0.2);
            color: #4CAF50;
        }
        
        .status-brewing {
            background: rgba(211, 47, 47, 0.2);
            color: #D32F2F;
            animation: pulse 1.5s ease-in-out infinite;
        }
        
        .status-stopped {
            background: rgba(255, 152, 0, 0.2);
            color: #FF9800;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.6; }
        }
        
        .timer-display {
            font-size: 3.5em;
            font-weight: 700;
            color: #D32F2F;
            margin: 16px 0;
            font-variant-numeric: tabular-nums;
            text-shadow: 0 2px 16px rgba(211, 47, 47, 0.3);
        }
        
        .timer-label {
            color: #666;
            font-size: 0.75em;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        
        .control-card {
            background: linear-gradient(135deg, #2d2d2d 0%, #3a3a3a 100%);
            border-radius: 16px;
            padding: 24px;
            border: 1px solid rgba(255, 255, 255, 0.05);
        }
        
        .control-label {
            color: #aaa;
            font-size: 0.9em;
            margin-bottom: 12px;
            display: block;
        }
        
        .input-group {
            display: flex;
            gap: 12px;
            margin-bottom: 20px;
        }
        
        input[type="number"] {
            flex: 1;
            background: rgba(50, 50, 50, 0.8);
            border: 1px solid rgba(255, 255, 255, 0.1);
            border-radius: 12px;
            padding: 14px 16px;
            color: #e0e0e0;
            font-size: 1.1em;
            text-align: center;
            transition: all 0.3s ease;
        }
        
        input[type="number"]:focus {
            outline: none;
            border-color: #D32F2F;
            box-shadow: 0 0 0 3px rgba(211, 47, 47, 0.1);
        }
        
        .slider-container {
            margin-bottom: 24px;
        }
        
        input[type="range"] {
            width: 100%;
            height: 6px;
            border-radius: 3px;
            background: rgba(255, 255, 255, 0.1);
            outline: none;
            -webkit-appearance: none;
        }
        
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background: #D32F2F;
            cursor: pointer;
            box-shadow: 0 2px 8px rgba(211, 47, 47, 0.4);
            transition: all 0.2s ease;
        }
        
        input[type="range"]::-webkit-slider-thumb:hover {
            transform: scale(1.2);
            box-shadow: 0 4px 16px rgba(211, 47, 47, 0.6);
        }
        
        input[type="range"]::-moz-range-thumb {
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background: #D32F2F;
            cursor: pointer;
            border: none;
            box-shadow: 0 2px 8px rgba(211, 47, 47, 0.4);
        }
        
        button {
            width: 100%;
            background: linear-gradient(135deg, #D32F2F 0%, #B71C1C 100%);
            color: white;
            border: none;
            border-radius: 12px;
            padding: 16px;
            font-size: 1.1em;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
            box-shadow: 0 4px 16px rgba(211, 47, 47, 0.3);
        }
        
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 24px rgba(211, 47, 47, 0.4);
        }
        
        button:active {
            transform: translateY(0);
        }
        
        .range-labels {
            display: flex;
            justify-content: space-between;
            color: #666;
            font-size: 0.8em;
            margin-top: 8px;
        }
        
        @media (max-width: 480px) {
            .container {
                padding: 24px 16px;
            }
            
            h1 {
                font-size: 1.6em;
            }
            
            .timer-display {
                font-size: 2.8em;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1><span class="brand">La Marcello</span> Shot Timer</h1>
        <p class="subtitle">E61 Shot Timer System</p>
        
        <div class="status-card">
            <div class="status-label">Current Status</div>
            <div id="status" class="status-value status-idle">IDLE</div>
            <div id="timer" class="timer-display">)rawliteral";

    html += timer_duration_millis / 1000;

    html += R"rawliteral(.0</div>
            <div class="timer-label">SECONDS</div>
        </div>
        
        <div class="control-card">
            <label class="control-label">Shot Duration</label>
            
            <div class="slider-container">
                <input type="range" id="sliderInput" min="1" max="60" value=")rawliteral";

    html += timer_duration_millis / 1000;

    html += R"rawliteral(" oninput="syncSlider(this.value)">
                <div class="range-labels">
                    <span>1s</span>
                    <span>30s</span>
                    <span>60s</span>
                </div>
            </div>
            
            <div class="input-group">
                <input type="number" id="numberInput" min="1" max="120" value=")rawliteral";

    html += timer_duration_millis / 1000;

    html += R"rawliteral(" oninput="syncNumber(this.value)">
            </div>
            
            <button onclick="setTimer()">Set Timer</button>
        </div>
    </div>
    
    <script>
        function syncSlider(value) {
            document.getElementById('numberInput').value = value;
        }
        
        function syncNumber(value) {
            const slider = document.getElementById('sliderInput');
            if (value >= slider.min && value <= slider.max) {
                slider.value = value;
            }
        }
        
        function setTimer() {
            const value = document.getElementById('numberInput').value;
            window.location.href = '/set?timer=' + value;
        }
        
        function updateStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    const statusEl = document.getElementById('status');
                    const timerEl = document.getElementById('timer');
                    
                    statusEl.textContent = data.state;
                    statusEl.className = 'status-value status-' + data.state.toLowerCase();
                    
                    if (data.state === 'BREWING') {
                        timerEl.textContent = data.remaining.toFixed(1);
                    } else {
                        timerEl.textContent = data.timerDuration.toFixed(1);
                    }
                })
                .catch(err => console.error('Error fetching status:', err));
        }
        
        // Update status every 100ms for smooth countdown
        setInterval(updateStatus, 100);
        
        // Initial update
        updateStatus();
    </script>
</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

// Handler for setting timer (/set)
void handleSetTimer()
{
    if (server.hasArg("timer"))
    {
        int newTimer = server.arg("timer").toInt();

        // Validate range
        if (newTimer >= 1 && newTimer <= 120)
        {
            timer_duration_millis = newTimer * 1000;

            Serial.print("Timer updated to: ");
            Serial.print(newTimer);
            Serial.println(" seconds");
        }

        server.sendHeader("Location", "/");
        server.send(303);
    }
    else
    {
        server.send(400, "text/plain", "Missing timer parameter");
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(MICROSWITCH_PIN, INPUT);
    pinMode(SSR_CONTROL_PIN, OUTPUT);
    digitalWrite(SSR_CONTROL_PIN, LOW);

    WiFi.begin(WIFI_SSID, WIFI_PWD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Register handlers
    server.on("/", handleRoot);
    server.on("/set", handleSetTimer);
    server.on("/status", handleStatus); // New endpoint!

    server.begin();
    Serial.println("Web server started!");
}

void loop()
{
    server.handleClient();
    switch_pressed = digitalRead(MICROSWITCH_PIN);

    switch (current_state)
    {
    case IDLE:
        if (switch_pressed)
        {
            start_time = millis();
            digitalWrite(SSR_CONTROL_PIN, HIGH);
            current_state = BREWING;
            Serial.println("Started Brewing!");
        }
        break;

    case BREWING:
        if (!switch_pressed || millis() - start_time >= timer_duration_millis)
        {
            digitalWrite(SSR_CONTROL_PIN, LOW);
            current_state = STOPPED;

            if (!switch_pressed)
            {
                Serial.println("Stopped: Switch Released Early!");
            }
            else
            {
                Serial.println("Stopped: Timer over");
            }
        }
        break;

    case STOPPED:
        if (!switch_pressed)
        {
            current_state = IDLE;
            Serial.println("Switch finally released! Going back to idle. Ready for next shot");
        }
        break;
    }
}