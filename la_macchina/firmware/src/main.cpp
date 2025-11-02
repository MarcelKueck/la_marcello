#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "config.h"

enum State
{
    IDLE,
    PRE_INFUSION,
    BREWING,
    STOPPED
};

//Timer Variables
State current_state = IDLE;
bool switch_pressed = false;
unsigned long start_time = 0;
unsigned long timer_duration_millis = DEFAULT_TIMER_SECONDS * 1000;

//Pre Infusion Variables
bool preinfusion_enabled = true;
unsigned long preinfusion_duration_millis = 5000;
unsigned long fill_pulse_duration_millis = 1000;
bool fill_pulse_completed = false;
unsigned long preinfusion_start_time = 0;

//Web Variables
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
    case PRE_INFUSION:
        json += "PRE_INFUSION";
        break;
    case BREWING:
        json += "BREWING";
        break;
    case STOPPED:
        json += "STOPPED";
        break;
    }

    json += "\",";
    json += "\"preinfusionEnabled\":";
    json += preinfusion_enabled ? "true" : "false";
    json += ",";
    json += "\"preinfusionDuration\":";
    json += preinfusion_duration_millis / 1000;
    json += ",";
    json += "\"fillPulseDuration\":";
    json += fill_pulse_duration_millis / 1000;
    json += ",";
    json += "\"extractionDuration\":";
    json += timer_duration_millis / 1000;
    json += ",";
    json += "\"elapsed\":";

    if (current_state == PRE_INFUSION)
    {
        unsigned long elapsed = (millis() - preinfusion_start_time) / 1000;
        json += elapsed;
    }
    else if (current_state == BREWING)
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

    if (current_state == PRE_INFUSION)
    {
        unsigned long elapsed = millis() - preinfusion_start_time;
        if (elapsed < preinfusion_duration_millis)
        {
            json += (preinfusion_duration_millis - elapsed) / 1000;
        }
        else
        {
            json += "0";
        }
    }
    else if (current_state == BREWING)
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
        
        .status-pre_infusion {
            background: rgba(33, 150, 243, 0.2);
            color: #2196F3;
            animation: pulse 1.5s ease-in-out infinite;
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
        
        .timer-display.preinfusion {
            color: #2196F3;
            text-shadow: 0 2px 16px rgba(33, 150, 243, 0.3);
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
            margin-bottom: 16px;
            border: 1px solid rgba(255, 255, 255, 0.05);
        }
        
        .control-label {
            color: #aaa;
            font-size: 0.9em;
            margin-bottom: 12px;
            display: block;
        }
        
        .toggle-container {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
            padding: 12px 0;
        }
        
        .toggle-label {
            color: #e0e0e0;
            font-size: 1em;
            font-weight: 500;
        }
        
        .toggle-switch {
            position: relative;
            display: inline-block;
            width: 56px;
            height: 28px;
        }
        
        .toggle-switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        
        .toggle-slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: rgba(255, 255, 255, 0.1);
            transition: 0.3s;
            border-radius: 28px;
        }
        
        .toggle-slider:before {
            position: absolute;
            content: "";
            height: 20px;
            width: 20px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: 0.3s;
            border-radius: 50%;
        }
        
        input:checked + .toggle-slider {
            background-color: #D32F2F;
        }
        
        input:checked + .toggle-slider:before {
            transform: translateX(28px);
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
        
        .preinfusion-settings {
            opacity: 1;
            max-height: 500px;
            transition: all 0.3s ease;
            overflow: hidden;
        }
        
        .preinfusion-settings.disabled {
            opacity: 0.4;
            max-height: 0;
            pointer-events: none;
        }
        
        .section-divider {
            height: 1px;
            background: rgba(255, 255, 255, 0.1);
            margin: 24px 0;
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
            <div id="timerLabel" class="timer-label">EXTRACTION TIME</div>
        </div>
        
        <div class="control-card">
            <div class="toggle-container">
                <span class="toggle-label">Pre-Infusion</span>
                <label class="toggle-switch">
                    <input type="checkbox" id="preinfusionToggle" )rawliteral";

    if (preinfusion_enabled)
        html += "checked";

    html += R"rawliteral(>
                    <span class="toggle-slider"></span>
                </label>
            </div>
            
            <div id="preinfusionSettings" class="preinfusion-settings">
                <label class="control-label">Pre-Infusion Time</label>
                <div class="slider-container">
                    <input type="range" id="preinfusionSlider" min="0" max="10" step="0.5" value=")rawliteral";

    html += String(preinfusion_duration_millis / 1000.0, 1);

    html += R"rawliteral(" oninput="syncPreinfusionSlider(this.value)">
                    <div class="range-labels">
                        <span>0s</span>
                        <span>5s</span>
                        <span>10s</span>
                    </div>
                </div>
                
                <div class="input-group">
                    <input type="number" id="preinfusionInput" min="0" max="10" step="0.5" value=")rawliteral";

    html += String(preinfusion_duration_millis / 1000.0, 1);

    html += R"rawliteral(" oninput="syncPreinfusionNumber(this.value)">
                </div>
                
                <label class="control-label">Fill Pulse Duration</label>
                <div class="slider-container">
                    <input type="range" id="fillPulseSlider" min="0" max="2" step="0.1" value=")rawliteral";

    html += String(fill_pulse_duration_millis / 1000.0, 1);

    html += R"rawliteral(" oninput="syncFillPulseSlider(this.value)">
                    <div class="range-labels">
                        <span>0s</span>
                        <span>1s</span>
                        <span>2s</span>
                    </div>
                </div>
                
                <div class="input-group">
                    <input type="number" id="fillPulseInput" min="0" max="2" step="0.1" value=")rawliteral";

    html += String(fill_pulse_duration_millis / 1000.0, 1);

    html += R"rawliteral(" oninput="syncFillPulseNumber(this.value)">
                </div>
                
                <div class="section-divider"></div>
            </div>
            
            <label class="control-label">Extraction Time</label>
            
            <div class="slider-container">
                <input type="range" id="extractionSlider" min="1" max="60" value=")rawliteral";

    html += timer_duration_millis / 1000;

    html += R"rawliteral(" oninput="syncExtractionSlider(this.value)">
                <div class="range-labels">
                    <span>1s</span>
                    <span>30s</span>
                    <span>60s</span>
                </div>
            </div>
            
            <div class="input-group">
                <input type="number" id="extractionInput" min="1" max="120" value=")rawliteral";

    html += timer_duration_millis / 1000;

    html += R"rawliteral(" oninput="syncExtractionNumber(this.value)">
            </div>
            
            <button onclick="saveSettings()">Save Settings</button>
        </div>
    </div>
    
    <script>
        // Sync functions for pre-infusion
        function syncPreinfusionSlider(value) {
            document.getElementById('preinfusionInput').value = value;
        }
        
        function syncPreinfusionNumber(value) {
            const slider = document.getElementById('preinfusionSlider');
            if (value >= slider.min && value <= slider.max) {
                slider.value = value;
            }
        }
        
        // Sync functions for fill pulse
        function syncFillPulseSlider(value) {
            document.getElementById('fillPulseInput').value = value;
        }
        
        function syncFillPulseNumber(value) {
            const slider = document.getElementById('fillPulseSlider');
            if (value >= slider.min && value <= slider.max) {
                slider.value = value;
            }
        }
        
        // Sync functions for extraction
        function syncExtractionSlider(value) {
            document.getElementById('extractionInput').value = value;
        }
        
        function syncExtractionNumber(value) {
            const slider = document.getElementById('extractionSlider');
            if (value >= slider.min && value <= slider.max) {
                slider.value = value;
            }
        }
        
        // Toggle pre-infusion settings visibility
        document.getElementById('preinfusionToggle').addEventListener('change', function() {
            const settings = document.getElementById('preinfusionSettings');
            if (this.checked) {
                settings.classList.remove('disabled');
            } else {
                settings.classList.add('disabled');
            }
        });
        
        // Initialize visibility on page load
        if (!document.getElementById('preinfusionToggle').checked) {
            document.getElementById('preinfusionSettings').classList.add('disabled');
        }
        
        function saveSettings() {
            const preinfusionEnabled = document.getElementById('preinfusionToggle').checked ? 1 : 0;
            const preinfusionTime = document.getElementById('preinfusionInput').value;
            const fillPulseTime = document.getElementById('fillPulseInput').value;
            const extractionTime = document.getElementById('extractionInput').value;
            
            window.location.href = '/set?preinfusion=' + preinfusionEnabled + 
                                   '&preinfusionTime=' + preinfusionTime + 
                                   '&fillPulse=' + fillPulseTime + 
                                   '&extraction=' + extractionTime;
        }
        
        function updateStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    const statusEl = document.getElementById('status');
                    const timerEl = document.getElementById('timer');
                    const labelEl = document.getElementById('timerLabel');
                    
                    // Update status badge
                    statusEl.textContent = data.state.replace('_', '-');
                    statusEl.className = 'status-value status-' + data.state.toLowerCase();
                    
                    // Update timer display and label based on state
                    if (data.state === 'PRE_INFUSION') {
                        timerEl.textContent = data.remaining.toFixed(1);
                        timerEl.className = 'timer-display preinfusion';
                        labelEl.textContent = 'PRE-INFUSION';
                    } else if (data.state === 'BREWING') {
                        timerEl.textContent = data.remaining.toFixed(1);
                        timerEl.className = 'timer-display';
                        labelEl.textContent = 'EXTRACTION';
                    } else {
                        timerEl.textContent = data.extractionDuration.toFixed(1);
                        timerEl.className = 'timer-display';
                        labelEl.textContent = 'EXTRACTION TIME';
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

// Handler for setting timer and pre-infusion parameters (/set)
void handleSetTimer()
{
    bool hasUpdates = false;

    // Handle pre-infusion enabled/disabled
    if (server.hasArg("preinfusion"))
    {
        preinfusion_enabled = (server.arg("preinfusion").toInt() == 1);
        hasUpdates = true;
    }

    // Handle pre-infusion time
    if (server.hasArg("preinfusionTime"))
    {
        float preinfusionTime = server.arg("preinfusionTime").toFloat();
        if (preinfusionTime >= 0 && preinfusionTime <= 10)
        {
            preinfusion_duration_millis = preinfusionTime * 1000;
            hasUpdates = true;
        }
    }

    // Handle fill pulse duration
    if (server.hasArg("fillPulse"))
    {
        float fillPulse = server.arg("fillPulse").toFloat();
        if (fillPulse >= 0 && fillPulse <= 2)
        {
            fill_pulse_duration_millis = fillPulse * 1000;
            hasUpdates = true;
        }
    }

    // Handle extraction time
    if (server.hasArg("extraction"))
    {
        int extractionTime = server.arg("extraction").toInt();
        if (extractionTime >= 1 && extractionTime <= 120)
        {
            timer_duration_millis = extractionTime * 1000;
            hasUpdates = true;
        }
    }

    // Legacy support for old "timer" parameter
    if (server.hasArg("timer"))
    {
        int newTimer = server.arg("timer").toInt();
        if (newTimer >= 1 && newTimer <= 120)
        {
            timer_duration_millis = newTimer * 1000;
            hasUpdates = true;
        }
    }

    if (hasUpdates)
    {
        server.sendHeader("Location", "/");
        server.send(303);
    }
    else
    {
        server.send(400, "text/plain", "No valid parameters");
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(MICROSWITCH_PIN, INPUT_PULLDOWN); //Prevent floating state
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
            preinfusion_start_time = millis();
            fill_pulse_completed = false;
            digitalWrite(SSR_CONTROL_PIN, HIGH);
            current_state = PRE_INFUSION;
        }
        break;

    case PRE_INFUSION:
        if (!preinfusion_enabled || millis() - preinfusion_start_time >= preinfusion_duration_millis)
        {
            start_time = millis();
            digitalWrite(SSR_CONTROL_PIN, HIGH);
            current_state = BREWING;
        }
        else if (!switch_pressed)
        {
            digitalWrite(SSR_CONTROL_PIN, LOW);
            current_state = STOPPED;
        }
        else if (millis() - preinfusion_start_time >= fill_pulse_duration_millis && !fill_pulse_completed)
        {
            digitalWrite(SSR_CONTROL_PIN, LOW);
            fill_pulse_completed = true;
        }
        break;

    case BREWING:
        if (!switch_pressed || millis() - start_time >= timer_duration_millis)
        {
            digitalWrite(SSR_CONTROL_PIN, LOW);
            current_state = STOPPED;
        }
        break;

    case STOPPED:
        if (!switch_pressed)
        {
            current_state = IDLE;
        }
        break;
    }
}