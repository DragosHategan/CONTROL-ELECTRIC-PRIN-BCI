/**
 * Appliance Control
 * Function implementation file of the Appliance Control
 *
 * Copyright 2023 Tudor Dragos Hategan
 *
 * This file is part of Appliance Control.
 *
 * Appliance Control is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Appliance Control is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Appliance Control. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "Header.h"
#include "ClassesHeader.h"
#include "pico/multicore.h"
#include "hardware/adc.h"

void initializePICO(){
    // INITIALIZE PICO BASE FUNCTIONS
    initializeIO();
    initializeADC();
    initializeUsbConnection();
}

void initializeIO(){
    // INITIALIZE STANDARD I/O
    stdio_init_all();
}

void initializeADC(){
    // INITIALIZING ADC USAGE
    adc_init();
}

void initializeUsbConnection(){
    // INITIALIZING DATA FLOW VIA USB SERIAL
    stdio_usb_init();
}

void initializePin(int pin_number){
    // INITIALIZING THE GIVEN PIN FOR FURTHER MANIPULATION
    gpio_init(pin_number);
}

void initializePinAsINPUT(int pin_number){
    // INITIALIZING THE GIVEN PIN FOR FURTHER INPUT MANIPULATION
    gpio_init(pin_number);
    gpio_set_dir(pin_number, GPIO_IN);
}

void initializePinAsOUTPUT(int pin_number){
    // INITIALIZING THE GIVEN PIN FOR FURTHER OUTPUT MANIPULATION
    gpio_init(pin_number);
    gpio_set_dir(pin_number, GPIO_OUT);
}

void setPinAsOUTPUT(int pin_number){
    // SETS PIN FOR OUTPUT USE
    gpio_set_dir(pin_number, GPIO_OUT);
}

void setPinAsINPUT(int pin_number){
    // SETS PIN FOR INPUT USE
    gpio_set_dir(pin_number, GPIO_IN);
}

void setPinOFF(int pin_number){
    // TURNS PIN OFF
    gpio_put(pin_number, 0);
}

void setPinON(int pin_number){
    // TURNS PIN ON
    gpio_put(pin_number, 1);
}

void setStateForPin(int state, int pin_number){
    // SETING A PIN 'ON' OR 'OFF'
    gpio_set_dir(pin_number, GPIO_OUT);
    gpio_put(pin_number, state);
}

void initializeTemperatureSensor(){
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
}

void sendTemperatureEverySecond_ON_CORE_1(){
    multicore_launch_core1(sendTemperatureEverySecond);
}

void sendTemperatureEverySecond(){
    initializeTemperatureSensor();
    while (1){
        sendTemperature();
        sleep_ms(1000);
    }
}

void sendTemperature(){
        uint16_t raw = adc_read();
        const float conversion = 3.3f / (1<<12);
        float voltage = raw * conversion;
        float temperature = 27 - (voltage - 0.706) / 0.001721;
        printf("Temperature: %.1f°C\n", temperature);
}
