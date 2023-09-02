/**
 * Appliance Control
 * Main header file of the Appliance Control
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

#ifndef APPLIANCE_TOTAL_NUMBER 
#define APPLIANCE_TOTAL_NUMBER 7
#endif

#ifndef HEADER_H
#define HEADER_H

void initializePICO();

void initializeIO();

void initializeADC();

void initializeUsbConnection();

void initializePin(int pin_number);

void initializePinAsINPUT(int pin_number);

void initializePinAsOUTPUT(int pin_number);

void setPinAsOUTPUT(int pin_number);

void setPinAsINPUT(int pin_number);

void setPinOFF(int pin_number);

void setPinON(int pin_number);

void setStateForPin(int state, int pin_number);

void initializeTemperatureSensor();

void sendTemperatureEverySecond_ON_CORE_1();

void sendTemperatureEverySecond();

void sendTemperature();


#endif // HEADER_H
