#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <AfricasTalkingCloud.h>
#include "DHT.h"

#define DELAY_DURATION 60
uint8_t dht_pin = 15; // ESP GPIO 15 ? 12
uint8_t led_pin = 12;
uint8_t led_status = 0;
uint8_t mqtt_qos = 1; // Quality of Service to use for the MQTT client-server connection
const char wifi_ssid[] = "our_cool_wifi";
const char wifi_password[] = "our_secret_password";
const char username[] = "kabarak"; // MQTT username for the demo
const char password[] = "kabarak"; // MQTT authorised user password
const char temp_topic[] = "kabarak/esp8266/temp"; // MQTT topic to send temperature data
const char humidity_topic[] = "kabarak/esp8266/humidity"; // MQTT topic to send humidity data
const char command_topic[] = "kabarak/esp8266/commands"; // MQTT topic through which commands will be sent
const char birth_topic[] = "kabarak/esp8266/birth"; //  MQTT topic to publish when the device reconnects
const char client_id[] = "esp8266"; // Fully qulaified Client ID used for the connection (mandatory)

void connectToWifi(void);
void setup(void);
void loop(void);
void sendTemperature(float temp);
void sendHumidity(float humidity);
void espCallBak(char* topic, byte* payload, unsigned int length);

WiFiClient esp8266cli; // Our Networking client instance
AfricasTalkingCloudClient client(espCallBak, esp8266cli);
DHT dht;

void setup()
{
    connectToWifi();
    dht.setup(dht_pin);
}

void loop()
{
    while(!client.connected()){
        client.connect(client_id, username, password);
        client.publish(birth_topic,"birth");
        client.subscribe(command_topic);
    }
    float temp = dht.getTemperature();
    float humidity = dht.getHumidity();
    sendHumidity(humidity);
    sendTemperature(temp);
    int counter_ = DELAY_DURATION;
    while(counter_ --)
    {
        delay(250);
    }
    
}

void sendHumidity(float humidity)
{
    float humidity_ = humidity;

    while(!client.connected()){
        client.connect(client_id, username, password);
        client.publish(birth_topic,"birth");
        client.subscribe(command_topic);
    }
    char data[20];
    snprintf(data, 30, "%f", humidity_);
    client.publish(humidity_topic, data, mqtt_qos);
    
}

void sendTemperature(float temperature)
{
    float temperature_ = temperature;
    while(!client.connected()){
        client.connect(client_id, username, password);
        client.publish(birth_topic,"birth");
        client.subscribe(command_topic);
    }
    char data[20];
    snprintf(data, 30, "%f", temperature_);
    client.publish(temp_topic, data, mqtt_qos);
}

void espCallBak(char* topic, byte* payload, unsigned int length)
{
    String command = ""; // Initialise with empty char array : populate later
    // Append char from payload to from payload
    
    for(uint8_t i = 0; i < length; i++)
    {
        command += (char)payload[i];
    }
    
    if ((topic == command_topic) && (command == "ON")) {
        led_status = 1;
        digitalWrite(led_pin, led_status);
    }
    else if((topic == command_topic) && (command == "OFF")) {
        led_status = 0;
        digitalWrite(led_pin, led_status);
    }
    else {
        digitalWrite(led_pin, led_status);
    }
    
    
}

void connectToWifi()
{
    WiFi.begin(wifi_ssid, wifi_password);
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        digitalWrite(led_pin,1);
        delay(500);
        digitalWrite(led_pin,0);
    }
}

