#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "config.h"

enum State {
    IDLE,
    BREWING,
    STOPPED
};

State current_state = IDLE;
bool switch_pressed = false;
unsigned long start_time = 0;
unsigned long timer_duration_millis = DEFAULT_TIMER_SECONDS * 1000;

WebServer server(WEB_SERVER_PORT);

// Handler for root page (/)
void handleRoot()
{
    // Build HTML page
    String html = "<html><body>";
    html += "<h1>E61 Flow Control</h1>";
    html += "<p>Current timer: ";
    html += timer_duration_millis / 1000;
    html += " seconds</p>";

    // Add a form
    html += "<form action='/set' method='GET'>";
    html += "New timer (seconds): <input type='number' name='timer' min='1' max='120' value='";
    html += timer_duration_millis / 1000;
    html += "'>";
    html += "<input type='submit' value='Set Timer'>";
    html += "</form>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

// Handler for setting timer (/set)
void handleSetTimer()
{
    // Check if 'timer' parameter exists
    if (server.hasArg("timer"))
    {
        // Get the value
        int newTimer = server.arg("timer").toInt();

        // Update the global timer variable
        timer_duration_millis = newTimer * 1000;

        Serial.print("Timer updated to: ");
        Serial.print(newTimer);
        Serial.println(" seconds");

        // Redirect back to root page
        server.sendHeader("Location", "/");
        server.send(303);
    }
    else
    {
        server.send(400, "text/plain", "Missing timer parameter");
    }
}

void setup() {
    Serial.begin(115200);

    //Set Pin Modes
    pinMode(MICROSWITCH_PIN, INPUT);
    pinMode(SSR_CONTROL_PIN, OUTPUT);

    //Make sure Relay is open so that Pump is turned off
    digitalWrite(SSR_CONTROL_PIN, LOW);

    //Connect to wifi
    WiFi.begin(WIFI_SSID, WIFI_PWD);

    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Register handlers
    server.on("/", handleRoot);
    server.on("/set", handleSetTimer);

    // Start server
    server.begin();
    Serial.println("Web server started!");
}

void loop() {
    //Handle Web Server Request
    server.handleClient();

    //Read Microswitch State
    switch_pressed = digitalRead(MICROSWITCH_PIN);

    switch(current_state){

        case IDLE:
            //Switch Pressed -> Close relay and activate pump
            if (switch_pressed){
                start_time = millis();
                digitalWrite(SSR_CONTROL_PIN, HIGH);
                current_state = BREWING;
                Serial.println("Started Brewing!");
            }
            break;
        case BREWING:
            //When timer over or switch release -> Open Relay and stop pump and change state
            if (!switch_pressed || millis() - start_time >= timer_duration_millis){
                digitalWrite(SSR_CONTROL_PIN, LOW);
                current_state = STOPPED;

                if (!switch_pressed){
                    Serial.println("Stopped: Switch Released Early!");
                }else {
                    Serial.println("Stopped: Timer over");
                }
            }
            break;
        case STOPPED:
            if (!switch_pressed){
                current_state = IDLE;
                Serial.println("Switch finally released! Going back to idle. Ready for next shot");
            }
            break;
    }

}