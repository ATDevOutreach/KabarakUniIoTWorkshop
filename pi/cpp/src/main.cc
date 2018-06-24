#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/client.h"
#include "mraa/common.hpp"
#include "mraa/aio.hpp"

#define LDR_PIN NN // TBD
#define ULTRA_PIN NN // TBD
#define LED_MAIN_ROOM_PIN NN //TBD
#define LED_MAIN_ROOM_AUX_PIN NN //TBD
