/**
 * Appliance Control
 * Class file of the Appliance Control
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

#include <string.h>
#include <stdio.h>
#include "Header.h"
#include "ClassesHeader.h"

class State{

};

// ELECTRICAL_APPLIANCE IMPLEMENTATION
ElectricalAppliance::ElectricalAppliance(unsigned int pin_number) {
    Pin_Number = pin_number;
}

unsigned int ElectricalAppliance::getPinNumber() {
    return Pin_Number;
}

void ElectricalAppliance::setPinNumber(unsigned int pin_number) {
    Pin_Number = pin_number;
    initializePin(Pin_Number);
    // Add any necessary logic here
}

const char* ElectricalAppliance::getData() {
    static char result[75];
    snprintf(result, sizeof(result), "SimpleElectricalAppliance, Pin:%d.", Pin_Number);
    return result;
}


// OUTPUT_ELECTRICAL_APPLIANCE IMPLEMENTATION
OutputElectricalAppliance::OutputElectricalAppliance(unsigned int pin_number, bool direction, bool state)
    : ElectricalAppliance(pin_number), State(state), Direction(direction) {
    State = state;
    Direction = direction;
    initializePinAsOUTPUT(getPinNumber());
    if (State == true){
            setPinON(getPinNumber());
    }
    else{
        setPinOFF(getPinNumber());
    }
}

bool OutputElectricalAppliance::getState() {
    return State;
}

unsigned int OutputElectricalAppliance::getDirection() {
    return Direction;
}

void OutputElectricalAppliance::setState(bool state) {
    State = state;
    if (State == true){
        setPinON(getPinNumber());
    }
    else if (State == false){
        setPinOFF(getPinNumber());
    }
}

const char* OutputElectricalAppliance::getData() {
    static char result[75];
    snprintf(result, sizeof(result), "OutputElectricalAppliance, Pin:%d, Direction:%d, State:%d.", getPinNumber(), Direction, State);
    return result;
}


// INPUT_ELECTRICAL_APPLIANCE IMPLEMENTATION
InputElectricalAppliance::InputElectricalAppliance(unsigned int pin_number, bool direction, bool state)
    : ElectricalAppliance(pin_number), State(state), Direction(direction) {
    State = state;
    Direction = direction;
    initializePinAsINPUT(getPinNumber());
    if (State == true){
        setPinON(getPinNumber());
    }
    else{
        setPinOFF(getPinNumber());
    }
}

bool InputElectricalAppliance::getState() {
    return State;
}

unsigned int InputElectricalAppliance::getDirection() {
    return Direction;
}

void InputElectricalAppliance::readInfo(){

}

void InputElectricalAppliance::setState(bool state) {
    State = state;
    if (State == true){
        setPinON(getPinNumber());
    }
    else if (State == false){
        setPinOFF(getPinNumber());
        }
}

const char* InputElectricalAppliance::getData() {
    static char result[75];
    snprintf(result, sizeof(result), "InputElectricalAppliance, Pin:%d, Direction:%d, State:%d.", getPinNumber(), Direction, State);
    return result;
}


// PWM_APPLIANCE IMPLEMENTATION
PWM_Appliance::PWM_Appliance(unsigned int pin_number, unsigned int frequency, unsigned int duty_cycle)
    : ElectricalAppliance(pin_number), Frequency(frequency), Duty_Cycle(duty_cycle) {
    Frequency = frequency;
    Duty_Cycle = duty_cycle;
}

unsigned int PWM_Appliance::getFrequency() {
    return Frequency;
}

unsigned int PWM_Appliance::getDuty_Cycle() {
    return Duty_Cycle;
}

void PWM_Appliance::setFrequency(unsigned int frequency) {
    Frequency = frequency;
}

void PWM_Appliance::setDuty_Cycle(unsigned int duty_cycle) {
    Duty_Cycle = duty_cycle;
}

const char* PWM_Appliance::getData() {
    static char result[75];
    snprintf(result, sizeof(result), "PWM_Appliance, Pin:%d, Frequency:%d, Duty_Cycle:%d.", getPinNumber(), Frequency, Duty_Cycle);
    return result;
}

//     void setFrequency(unsigned int frequency){
//         Frequency = frequency;
//     }

//     void setDuty_Cycle(unsigned int duty_cycle){
//         Duty_Cycle = duty_cycle;
//     }


// ADC_APPLIANCE IMPLEMENTATION
ADC_Appliance::ADC_Appliance(unsigned int pin_number, unsigned int adc_input)
    : ElectricalAppliance(pin_number), ADC_Input(adc_input) {
    ADC_Input = adc_input;
}

const char* ADC_Appliance::getData() {
    static char result[75];
    snprintf(result, sizeof(result), "ADC_Appliance, Pin:%d, ADC_Input:%d.", getPinNumber(), ADC_Input);
    return result;
}

// class ADC_Appliance:public ElectricalAppliance{
// private :
//     unsigned int ADC_Input;
// public:
//     ADC_Appliance(unsigned int pin_number, unsigned int adc_input): ElectricalAppliance(pin_number){
//         ADC_Input = adc_input;
//     }
