/**
 * Appliance Control
 * Class header file of the Appliance Control
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

#ifndef CLASSES_HEADER_H
#define CLASSES_HEADER_H

class ElectricalAppliance {
private:
    unsigned int Pin_Number; // PIN ID NUMBER IN MICROCONTROLLER

public:
    ElectricalAppliance(unsigned int pin_number);
    unsigned int getPinNumber();
    void setPinNumber(unsigned int pin_number);
    virtual const char* getData();
};

class InputElectricalAppliance : public ElectricalAppliance {
private:
    bool State; // TRUE IS ON AND FALSE IS OFF
    bool Direction; // TRUE IS OUTPUT AND FALSE IS INPUT

public:
    InputElectricalAppliance(unsigned int pin_number, bool direction, bool state);
    bool getState();
    unsigned int getDirection();
    void setState(bool state);
    void readInfo();
    const char* getData() override;
};

class OutputElectricalAppliance : public ElectricalAppliance {
private:
    bool State; // TRUE IS ON AND FALSE IS OFF
    bool Direction; // TRUE IS OUTPUT AND FALSE IS INPUT

public:
    OutputElectricalAppliance(unsigned int pin_number, bool direction, bool state);
    bool getState();
    unsigned int getDirection();
    void setState(bool state);
    const char* getData() override;
};

class PWM_Appliance : public ElectricalAppliance {
private:
    unsigned int Frequency; // FREQUENCY OF THE ELECTRICAL PULSE TO THE CONSUMER
    unsigned int Duty_Cycle; // DUTY CYCLE OF THE CONSUMER, HOW MUCH TIME THE APPLIANCE IS ON PER CYCLE
public:
    PWM_Appliance(unsigned int pin_number, unsigned int frequency, unsigned int duty_cycle);
    unsigned int getFrequency();
    unsigned int getDuty_Cycle();
    void setFrequency(unsigned int frequency);
    void setDuty_Cycle(unsigned int duty_cycle);
    const char* getData() override;
};

class ADC_Appliance : public ElectricalAppliance {
private:
    unsigned int ADC_Input; //  INPUT CHANNEL NUMBER

public:
    ADC_Appliance(unsigned int pin_number, unsigned int adc_input);
    const char* getData() override;
};

#endif // CLASSES_HEADER_H
