#include <Arduino.h>
#include <SPI.h>
#include "LiquidCrystal.h"
#include "WiFi101.h"
#include "DHT.h"

// TODO 
// LCD 

// TODO 
// Manage Rooms

// TODO 
// handle message callback

// Digital pins
uint8_t led_pin_1 = 42;
uint8_t led_pin_2 = 43;
uint8_t led_pin_3 = 44;
uint8_t dht_pin = 45;
uint8_t pir_pin = 46;

// PWM pins
uint8_t servo_pin_1 = 11;
uint8_t servo_pin_2 = 12;
uint8_t servo_pin_3 = 13;

// Analog pins
#define LDR_PIN A2

// Ultrasonic pins
uint8_t trigger_pin = 36;
uint8_t echo_pin = 34;

//Topics
const char birth_topic[] = "kabarak/atmega/birth";
const char main_room_topic[] = "kabarak/atmega/room/door";
const char garage_door_topic[] = "kabarak/atmega/room/garage";
const char window_manage_topic[] = "kabarak/atmega/room/window";
const char temperature_topic[] = "kabarak/atmega/sensors/temp";
const char humidity_topic[] = "kabarak/atmega/sensors/humidity";
const char proximity_topic[] ="kabarak/atmega/sensors/proximity";
const char light_topic[] = "kabarak/atmega/sensors/light";
const char main_room_light[] = "kabarak/atmega/appliance/led_main";
const char bed_room_light[]= "kabarak/atmega/appliance/led_bed"; 

// Payload
char message[30];

// Other
const char ssid[] = "africastalking";
const char password[] = "password";
int status = WL_IDLE_STATUS;
uint8_t led_state = 0;
uint8_t mqtt_qos_level = 1;

// MQTT creds
const char mqtt_username[] = "kabarak";
const char mqtt_passowrd[] = "kabarak";
const char device_id[] = "megaatmega";


void connectToWAP(const char* ssid, const char* password);
void reconectToMQTT(void);
void wifi101CallBack(char* topic, byte* payload, unsigned int length);
// void publishHumidity(float humidity);
// void publishTemperature(float temperature);
// void publishProximity(float distance);
// void publishLightIntensisty(float intensity);
void publishMetric(float metric, const char* topic);
long getPromixityValues(void);
void manageRoom(const char* room_name, string roomState);
void loop(void);
void setup(void);
bool reconn = false;


WiFiClient client101;
AfricasTalkingCloudClient client(wifi101CallBack, client101);

void setup(void)
{
    WiFi.setPins(10, 3, 4);
    pinMode(pir_pin, INPUT);
    pinMode(led_pin_1, OUTPUT);
    pinMode(led_pin_2, OUTPUT);
    pinMode(led_pin_3, OUTPUT);
    pinMode(pir_pin, OUTPUT);
    pinMode(echop_pin, OUTPUT);
    pinMode(trigger_pin, INPUT);
    dht.setup(dht_pin);
    connectToWAP(ssid, password);
}

void loop(void)
{   
    if(reconn == false)
        connectToWAP(ssid, password); 
    while(!client.loop() || !client.connected()){
        reconectToMQTT();   
    }   
    
}

void publishMetric(float metric, const char* topic)
{
    float metric_ = metric;
    const char* topic_ = topic;
    snprintf(message, 40, "%f", metric_);
    client.publish(topic_, message, mqtt_qos_level);
}


long getPromixityValues()
{
    digitalWrite(trigger_pin,LOW);
    delayMicroseconds(5);
    digitalWrite(trigger_pin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin,LOW);
    long duration = pulseIn(echo_pin,HIGH);
    long distance_ = (duration/2)/29.1;
    return distance_;
}

void reconectToMQTT(void)
{
    while(!client.connected()){
        client.connect(device_id, mqtt_username, mqtt_passowrd);
        client.publish(birth_topic, "birth", mqtt_qos_level);
        client.subscribe(main_room_light);
        client.subscribe(bed_room_light);
        client.subscribe(light_topic); // For increasing intensity
        client.subscribe(main_room_topic);
        client.subscribe(garage_door_topic);
        client.subscribe(window_manage_topic); // For closing and opening windows   
    }   
}

void connectToWAP(const char* ssid, const char* password)
{   
    const char ssid_[] = ssid;
    const char w_pass_[] = password; 
    if (WiFi.status() == WL_NO_SHIELD) {
        while(true)
        {
        digitalWrite(led_pin_1, !led_state);
        delay(1000);
        digitalWrite(led_pin_1, led_state);
        delay(1000);
        }
    }
     while(WiFi.status() != WL_CONNECTED){
         status = WiFi.begin(ssid_, w_pass_);
         delay(10000);
         reconn = true;
     }
}



