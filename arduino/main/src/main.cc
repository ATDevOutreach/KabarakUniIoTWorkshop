#include <Arduino.h>
#include <SPI.h>
#include "LiquidCrystal.h"
#include "WiFi101.h"
#include "DHT.h"
#include <Servo.h>
#include "AfricasTalkingCloud.h"


// Digital pins
uint8_t led_pin_1_main = 42; // main room light 1
uint8_t led_pin_2_aux = 43; // main aux light 2
uint8_t led_pin_4_main_colored = 41; // Colored lights
uint8_t led_pin_3_bed = 44; // bed room light
uint8_t dht_pin = 45;
uint8_t pir_pin = 46;
uint8_t buzzer_pin = 40;


// PWM pins
uint8_t servo_pin_1_garage = 11;
uint8_t servo_pin_2_main = 12;
uint8_t servo_pin_3_curtains= 13;

// Analog pins
#define LDR_PIN A2

// Ultrasonic pins
uint8_t trigger_pin = 36;
uint8_t echo_pin = 34;

//Topics
const char birth_topic[] = "kabarak/atmega/birth";
const char main_room_topic[] = "kabarak/atmega/room/main";
const char garage_door_topic[] = "kabarak/atmega/room/garage";
const char bed_room_topic[] = "kabarak/atmega/room/bed";
// const char window_manage_topic[] = "kabarak/atmega/room/bed/window";
const char temperature_topic[] = "kabarak/atmega/sensors/temp";
const char humidity_topic[] = "kabarak/atmega/sensors/humidity";
const char proximity_topic[] ="kabarak/atmega/sensors/proximity";
const char light_topic[] = "kabarak/atmega/sensors/light";
const char main_room_light[] = "kabarak/atmega/appliance/led_main";
// const char bed_room_light[]= "kabarak/atmega/appliance/room/bed/led_bed"; 
const char alarm_topic[] = "kabarak/atmega/appliance/room/main/alarm";
const char presence_topic[] = "kabarak/atmega/presence";

// Payload
char message[30];

// Other
const char ssid[] = {"africastalking"};
const char password[] = {"password"};
int status = WL_IDLE_STATUS;
uint8_t pin_state = 0;
uint8_t mqtt_qos_level = 1;
uint8_t lcd_pin = 5;

// Servo Control
uint8_t servo_pos = 0;
Servo main_motor;
Servo garage_motor;
Servo curtain_motor;

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
void publishMetric(float metric, char* topic);
float getPromixityValues(void);
void manageRoom(String room_name, String room_state);
void presenceHandler(int presence);
void proximityHandler(float proximity);
void motorPositionHandler(float proximity = NULL); // This might be controlled by the proximity values
boolean isRoomTopic(char topic[]); 
void alarmManager(void);
void loop(void);
void setup(void);
bool reconn = false;


WiFiClient client101;
AfricasTalkingCloudClient client(wifi101CallBack, client101);
DHT dht;
LiquidCrystal lcd(lcd_pin);

void setup(void)
{
    WiFi.setPins(10, 3, 4);
    pinMode(pir_pin, INPUT);
    pinMode(led_pin_1_main, OUTPUT);
    pinMode(led_pin_2_aux, OUTPUT);
    pinMode(led_pin_3_bed, OUTPUT);
    pinMode(led_pin_4_main_colored, OUTPUT);
    pinMode(pir_pin, OUTPUT);
    pinMode(echo_pin, OUTPUT);
    pinMode(trigger_pin, INPUT);
    dht.setup(dht_pin);
    connectToWAP(ssid, password);
    garage_motor.attach(servo_pin_1_garage);
    main_motor.attach(servo_pin_2_main);
    curtain_motor.attach(servo_pin_3_curtains);
    lcd.begin(20,4); // Assuming a 20x16 LCD
    lcd.print("Initlilized");

}

void loop(void)
{   
    
    if (WiFi.status() != WL_CONNECTED)
        reconn = false;
    if(reconn == false)
        connectToWAP(ssid, password); 

    while(!client.loop() || !client.connected()){
        reconectToMQTT();   
    }   
    
    // Collect Humidity , Temp, Presence (using PIR sensor), proximity and light intensity
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    int presence = digitalRead(pir_pin);
    float prox = getPromixityValues();
    float light_intensity = analogRead(LDR_PIN);
    char temp[50],h[50],li[50];
    // Stringify e'rything \_(-.-)_/
    snprintf(temp,40,"%f",temperature);
    snprintf(h,40,"%f",humidity);
    snprintf(li,40,"%f",light_intensity);
    // send everything to the server
    publishMetric(humidity, humidity_topic);
    publishMetric(temperature, temperature_topic);
    publishMetric(prox, proximity_topic);
    publishMetric(light_intensity, light_topic);
    presenceHandler(presence); // This is really bad, esssentially there should be some delay... but \_(-.-)_/
    // We can manually call the proximity handler here
    // proximityHandler(prox);
    // Display these stuff on the LCD screen
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Temp:");
    lcd.setCursor(1,1);
    lcd.print(temp);
    delay(1500);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Humidity:");
    lcd.setCursor(1,1);
    lcd.print(h);
    delay(1500);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("L.intensity:");
    lcd.setCursor(1,1);
    lcd.print(li);
    delay(1500);
    lcd.clear();

}

void presenceHandler(int presence)
{
    int presence_ = presence;
    presence_ = 1 ? client.publish(presence_topic, "TRUE", mqtt_qos_level) : client.publish(presence_topic, "FALSE", mqtt_qos_level); 
}

void manageRoom(String room_name, String room_state)
{
    String room,state;
    room = room_name;
    state = room_state;

        if(room == "main"){
            if (state ==  "PRESENCE") {
                // User enters room
                digitalWrite(led_pin_1_main, !pin_state);
                digitalWrite(led_pin_2_aux, !pin_state);
                digitalWrite(led_pin_4_main_colored, pin_state);
            }
            else if(state == "ABSENCE") {
                // User goes to another room (bed room)
                digitalWrite(led_pin_2_aux, pin_state);
            }
            else if(state == "OUT") {
                // User leaves room
                digitalWrite(led_pin_2_aux, pin_state);
                digitalWrite(led_pin_1_main, pin_state);
                digitalWrite(led_pin_4_main_colored, pin_state);
            } 
            else if (state == "ALLOFF") {
                digitalWrite(led_pin_1_main, pin_state);
                digitalWrite(led_pin_2_aux, pin_state);
                digitalWrite(led_pin_4_main_colored, pin_state);
            }
            else if (state == "COLORED") {
                digitalWrite(led_pin_4_main_colored, !pin_state);
            }
            else if (state == "SPEAK") {
                alarmManager();
            } else if(state == "INTENSITY") {
                digitalWrite(led_pin_1_main, !pin_state);
                digitalWrite(led_pin_2_aux, !pin_state);
                digitalWrite(led_pin_4_main_colored, !pin_state);
            }
            else {
                ;
            }
        }
        if(room == "bed")
        {
            if (state == "LIGHT") {
                digitalWrite(led_pin_3_bed, !pin_state);
            } else if(state == "DRAW") {
                for(servo_pos = 0; servo_pos < 180; servo_pos++)
                {
                    curtain_motor.write(servo_pos);
                    delay(15);
                }
            } else if(state == "UNDRAW") {
                servo_pos = 0; // Just ensure that the servo angle is 0, otherwise this could result into a huge bug
                int servo_cur_pos = curtain_motor.read(); // Get actual servo position : hard to tell [!INTERESTING]
                for(servo_cur_pos; servo_cur_pos > servo_pos; servo_cur_pos--)
                {
                    curtain_motor.write(servo_cur_pos);
                    delay(15);
                }
            } else if (state == "LIGHTOFF") {
                digitalWrite(led_pin_3_bed, pin_state);
            }            
            else {
                // Unimplemented
                ;
            }
        }
        if(room == "garage")
        {
            // This is governed by the proximity sensor
            // for the unsimulated  -- omit [!INTERESTING]
            if (state == "APPROACH") {
                motorPositionHandler();
            }
        }
}

void publishMetric(float metric, char* topic)
{
    float metric_ = metric;
    char* topic_ = topic;
    snprintf(message, 20, "%f", metric_);
    client.publish(topic_, message, mqtt_qos_level);
}


float getPromixityValues()
{
    digitalWrite(trigger_pin,LOW);
    delayMicroseconds(5);
    digitalWrite(trigger_pin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin,LOW);
    long duration = pulseIn(echo_pin,HIGH);
    float distance_ = (duration/2)/29.1;
    return distance_;
}

void reconectToMQTT(void)
{
    while(!client.connected()){
        client.connect(device_id, mqtt_username, mqtt_passowrd);
        client.publish(birth_topic, "birth", mqtt_qos_level);
        client.subscribe(main_room_light);
       // client.subscribe(bed_room_light);
        client.subscribe(light_topic); // For increasing intensity
        client.subscribe(main_room_topic);
        client.subscribe(garage_door_topic);
       // client.subscribe(window_manage_topic); // For closing and opening windows   
    }   
}

void connectToWAP(const char* ssid, const char* password)
{   
    const char* ssid_ = {ssid};
    const char* w_pass_ = {password}; 
    if (WiFi.status() == WL_NO_SHIELD) {
        while(true)
        {
        digitalWrite(led_pin_2_aux, !pin_state);
        delay(1000);
        digitalWrite(led_pin_2_aux, pin_state);
        delay(1000);
        }
    }
     while(WiFi.status() != WL_CONNECTED){
         status = WiFi.begin(ssid_, w_pass_);
         delay(10000);
         reconn = true;
         lcd.clear();
         lcd.setCursor(0,1);
         lcd.print("WiFi Status:");
         lcd.setCursor(1,1);
         lcd.print("Connected");
     }
}

void proximityHandler(float proximity)
{
    float proximity_ = proximity;
    motorPositionHandler(proximity_);
}

void motorPositionHandler(float proximity = NULL)
{
    int motor_angle = garage_motor.read();
    float proxim = proximity;
    boolean active = false;
    proxim < 15.00 ? active = true : active = false;
    if (motor_angle > 0 || active) {
                   
        for(motor_angle; motor_angle > 0; motor_angle--)
        {
            garage_motor.write(motor_angle);
            delay(15);
    
        } 
    }
    if (motor_angle < 180 || active) {
            servo_pos = 0 ; // [!INTERESTING]
            for(servo_pos; servo_pos < 180; servo_pos++)
            {
                garage_motor.write(servo_pos);
                delay(15);
            }
        }
}



void wifi101CallBack(char* topic, byte* payload, unsigned int length)
{
    // Get message packet and topic and pass to correct functions
    //1. Get topic
    //2. Room topic?
    //2.a. Route to func
    //3. Do something else
    String command = ""; // This is the MQTT payload we've received
    char* topic_ = {topic};
    
    for(unsigned int i = 0; i < length; i++)
    {
        command += (char)payload[i];
    }
    if(isRoomTopic(topic_))
    {
        // Switches in C can not handle anything beyond char and int
            if(topic_ == main_room_topic)
                manageRoom("main", command);
            if(topic_ == bed_room_topic)
                manageRoom("bed", command);
            if(topic_ == garage_door_topic)
                manageRoom("garage", command);
    }
    if (topic_ == alarm_topic) {
       alarmManager();
    }
    
}

boolean isRoomTopic(char topic[])
{
    String topic_ = (String)topic;
    topic_.substring(15) == "room";
    return true;
}

void alarmManager(void)
{
    digitalWrite(buzzer_pin, !pin_state);
    delay(7500);
    digitalWrite(buzzer_pin, pin_state);
}