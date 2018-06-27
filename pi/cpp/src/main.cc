#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/client.h"
#include "mraa.h"
#include "mraa/common.hpp"
#include "mraa/gpio.hpp"
#include "mraa/aio.hpp"

#define SERVO_PIN_DOOR 23 // TODO
#define SERVO_PIN_WINDOW 23 // TODO
#define LDR_PIN 24 // TODO
#define BUZZER_PIN 14 // TODO 
#define DHT_PIN 32 // TODO  
#define LED_PIN 33 // TODO
#define COLORED_LED_PIN 35 //TODO

const std::string MQTT_BROKER_URL("tcp://sungura1-angani-ke-host.africastalking.com:10883");
const std::string DEVICE_NAME("KABARAK_IOT");
const std::string DOOR_TOPIC("kabarak/lead/main/door");
const std::string ALARM_TOPIC("kabaraka/lead/main/alarm");
const std::string TEMP_TOPIC("kabarak/lead/main//sensor/temp");
const std::string HUMIDITY_TOPIC("kabarak/lead/main/sensor/humidity");
const std::string MAIN_LIGHT("kabarak/lead/main/light/main");
const std::string BIRTH_TOPIC("kabarak/lead/main/birth");
const std::string BIRTH_PAYLOAD("alive");
const std::string USERNAME("kabarak");
const std::string PASSWORD("kabarak");
std::string PAYLOAD("");
const uint8_t QOS = 1;

volatile sig_atomic_t flag = 1;

class callback : public virtual mqtt::callback
{
    mqtt::client& client_;
    void connected(const std::string& cause) override 
    {
        std::cout << "\nConnected:" << cause << std::endl;
        // Subscribe to topics
        client_.subscribe(MAIN_LIGHT, QOS);
        client_.subscribe(ALARM_TOPIC, QOS);
        client_.subscribe(DOOR_TOPIC, QOS);
        std::cout << std::endl;
    }

    void connection_lost(const std::string& cause) override 
    {
        std::cout << "\nConnection Lost";
        
        if (!cause.empty()) 
        {
            std::cout << ":" << cause << std::endl;
        }
        std::cout << std::endl;
    }
    void message_arrived(mqtt::const_message_ptr msg) override
    {
        std::cout << msg->get_topic() << ":" << msg->get_payload_str() << std::endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

    public:
     callback(mqtt::client& cli) : client_(cli) {}
};

void sig_handler(int signum)
{
    
    if (signum == SIGINT)
    {
        std::cout << "Exiting..." << std::endl;
        flag = 0;

    }
    
}

int main(void)
{
    mqtt::connect_options conOpts;
    conOpts.set_keep_alive_interval(20);
    conOpts.set_clean_session(false);
    conOpts.set_automatic_reconnect(true);
    conOpts.set_user_name(USERNAME);
    conOpts.set_password(PASSWORD);

    mqtt::client cli(MQTT_BROKER_URL, DEVICE_NAME);

    callback cb(cli);
    cli.set_callback(cb);

    try
    {
        std::cout << "Connecting to Server..." << std::flush;
        cli.connect(conOpts);
        std::cout << "Connection Success" << std::endl;
    }
    catch(const mqtt::exception& e)
    {
        std::cerr << "\nError Connecting to the Server" << '\n';
        return EXIT_FAILURE;
    }
    mraa::Result status;
    signal(SIGINT, sig_handler);
    mraa::Gpio gpio_main_led(LED_PIN);
    mraa::Gpio gpio_buzzer(BUZZER_PIN);
    mraa::Aio aio_ldr(LDR_PIN);
    float val = 0.00;

    mraa_pwm_context door_servo = mraa_pwm_init(SERVO_PIN_DOOR);
    mraa_pwm_enable(door_servo, 0);
    mraa_pwm_period(door_servo, 20);
    mraa_pwm_enable(door_servo, 1);

    status = gpio_main_led.dir(mraa::DIR_OUT);
    if (status != mraa::SUCCESS) 
    {
        printError(status);
        return EXIT_FAILURE;
    }

    status = gpio_buzzer.dir(mraa::DIR_OUT);
    
    if (status != mraa::SUCCESS) 
    {
        printError(status);
        return EXIT_FAILURE;
    }
    

    while (flag) {
  
        auto commands = cli.consume_message();

        
        if (commands->get_topic() == MAIN_LIGHT)
        {
            
            if (commands ->to_string() == "ON") {
                 status = gpio_main_led.write(1);
                if (status != mraa::SUCCESS) 
                {
                        printError(status);
                        return EXIT_FAILURE;
                }
                sleep(1);
            }
            else if(commands->to_string() == "OFF") {
                 status = gpio_main_led.write(0);
                if (status != mraa::SUCCESS) 
                {
                        printError(status);
                        return EXIT_FAILURE;
                }
                sleep(1);
            }
            else {
               break;
            }
        }
        
        if (commands->get_topic() == DOOR_TOPIC) {
            mraa_pwm_pulsewidth_us(door_servo, 20000-1000);
            sleep(4);
            mraa_pwm_pulsewidth_us(door_servo, 20000-1500);
            sleep(4);
            mraa_pwm_pulsewidth_us(door_servo, 20000-2000);
            sleep(4);
        }

        if (commands->get_topic() == ALARM_TOPIC) 
        {
            status = gpio_buzzer.write(1);
            if (status != mraa::SUCCESS) 
            {
                printError(status);
                return EXIT_FAILURE;
            }
            sleep(3);
            status = gpio_buzzer.write(0);
            if (status != mraa::SUCCESS) 
            {
                printError(status);
                return EXIT_FAILURE;
            }
        }
    }
    cli.disconnect();
    return EXIT_SUCCESS;
}