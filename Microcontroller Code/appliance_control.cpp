/**
 * Appliance Control
 * Main file of the Appliance Control
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


#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#include "Header.h"
#include "ClassesHeader.h"

#include <stdio.h>
#include <string.h>


void initializeOutputAppliances(OutputElectricalAppliance* appliances[]) {
    appliances[0] = new OutputElectricalAppliance(0, true, false);
    appliances[1] = new OutputElectricalAppliance(2, true, false);
    appliances[2] = new OutputElectricalAppliance(5, true, false);
    appliances[3] = new OutputElectricalAppliance(7, true, false);
    appliances[4] = new OutputElectricalAppliance(10, true, false);
    appliances[5] = new OutputElectricalAppliance(13, true, false);
    appliances[6] = new OutputElectricalAppliance(15, true, false);
}

void setOutputAppliance(int Pin, bool State, OutputElectricalAppliance* appliances[]){
    switch(Pin){
        case 0: appliances[0]->setState(State); break; 
        case 2: appliances[1]->setState(State); break; 
        case 5: appliances[2]->setState(State); break; 
        case 7: appliances[3]->setState(State); break; 
        case 10: appliances[4]->setState(State); break;
        case 13: appliances[5]->setState(State); break;
        case 15: appliances[6]->setState(State); break;
    }
}

void getOutputAppliancePinData(char ApplianceData[][50], OutputElectricalAppliance* appliances[]){
    
    int Pin = 0;
    bool Direction;
    bool State;

    if (strcmp(ApplianceData[0], "OutputElectricalAppliance") == 0){
        for (int index = 0; index < strlen(ApplianceData[1]); index++){
            if (ApplianceData[1][index] >= '0' && ApplianceData[1][index] <= '9'){
                Pin = Pin * 10 + ApplianceData[1][index] - '0';
            }
        }
        if (strstr(ApplianceData[3], "true")){
            State = true;
        }
        else{
            State = false;
        }
    }
    setOutputAppliance(Pin, State, appliances);
}

void getAttributesFromOutputAppliance(char stateLine[75], OutputElectricalAppliance* appliances[]){
    int data = 1;
    int counter = 0;
    char ApplianceData[4][50];
    char *strPointer;
    strPointer = strtok(stateLine, ", ");
    
    while (strPointer != NULL){
        strcpy(ApplianceData[counter], strPointer);
        strPointer = strtok(NULL, ", ");
        counter++;
    }
    getOutputAppliancePinData(ApplianceData, appliances);
}

void readOutputAppliancesCompleteStateFromPC(OutputElectricalAppliance* appliances[]){
    char stateLines[7][75];
    int index = 0;
    bool notRead = true;
    while (notRead){
        if(fgets(stateLines[index], sizeof(stateLines[index]), stdin)){
            index++;
            if (index == APPLIANCE_TOTAL_NUMBER){
                notRead = false;
            }
        }
    }
    for (index = 0; index < APPLIANCE_TOTAL_NUMBER; index++){
        getAttributesFromOutputAppliance(stateLines[index], appliances);
    }
}

void readOutputApplianceDataFromPC(OutputElectricalAppliance* appliances[]){
    bool notRead = true;
    while (notRead) {
        char string[75];
        if (fgets(string, sizeof(string), stdin)){
            getAttributesFromOutputAppliance(string, appliances);
            notRead = false;
        }
    }
}

void sendCurrentCompleteStateToPC(OutputElectricalAppliance* appliances[]){
    for (int i = 0; i < APPLIANCE_TOTAL_NUMBER; i++){
        printf("%s\n",appliances[i]->getData());
    }
}

void listenForCommands(OutputElectricalAppliance* appliances[]){
    char command[50];
    while (1){
        if (fgets(command, sizeof(command), stdin)){
            if (strcmp(command, "Change OutputAppliance\n") == 0){
                    readOutputApplianceDataFromPC(appliances);
            }
            else if (strcmp(command, "Change State\n") == 0){
                    readOutputAppliancesCompleteStateFromPC(appliances);
            }
            else if(strcmp(command, "Send State\n") == 0){
                sendCurrentCompleteStateToPC(appliances);
            }
            else if (strcmp(command, "Send Temperature\n") == 0){
                sendTemperature();
            }
        }
    }
}


int main(){
    initializePICO();
    initializeTemperatureSensor();
    OutputElectricalAppliance* appliances[APPLIANCE_TOTAL_NUMBER];
    initializeOutputAppliances(appliances);

    listenForCommands(appliances);
    
    return 0;
}

