#  Appliance Control
#  PC side file of the Appliance Control
# 
#  Copyright 2023 Tudor Dragos Hategan
# 
#  This file is part of Appliance Control.
# 
#  Appliance Control is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
# 
#  Appliance Control is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with Appliance Control. If not, see <https://www.gnu.org/licenses/>.

import customtkinter as ctk
import socket
import threading
import ctypes
import serial

# List used for appliance states
states = []

# Lists used for better GUI element manipulation
appliance_number_labels = []
appliance_state_labels = []
buttons = []

# Used for keyboard navigation
current_button_selection = 0

# Constants for MessageBox types
MB_OK = 0x0
MB_ICONINFORMATION = 0x40

# The entry point of the program, tries to connect to a device and configurates window
def start(root):
    # Window configuration
    main_frame = ctk.CTkFrame(master=root)
    config_window(root, main_frame)

    # Verifying and establishing connection
    connected = False
    exit = False
    while not exit:
        try:
            if not connected:
                # Conect to the microcontroller, only works if it is connected and no other app is using it on this port
                global serialObject
                serialObject = serial.Serial('COM15', 115200)
                connected = True

                # Gets necessary data for manipulating appliances
                receiveAppliancesStateFromMicrocontroller()
        except Exception:

            # Popup message if no device found
            ctypes.windll.user32.MessageBoxW(0, "Connect a device and press OK.", "No device found", MB_OK | MB_ICONINFORMATION)
        else:
            # Configuring and starting the frame
            toggle_page(root, main_frame)
            button_actions()
            root.mainloop()
            exit = True


# Window configuration
def config_window(root, main_frame):
    ctk.set_appearance_mode("dark")
    ctk.set_default_color_theme("dark-blue")

    root.title("Appliances Controller")
    root.after(201, lambda: root.iconbitmap('C:\\Users\\Dragos\\Desktop\\House Control System\\GUI\\led.jpeg'))

    root.geometry("1920x1080")  # Adjusted dimensions
    root.attributes("-fullscreen", True)  # Set to full screen mode

    main_frame.pack(pady=10, padx=10, fill="both", expand=True)


# Gets necessary data for manipulating appliances
def receiveAppliancesStateFromMicrocontroller():
    commandForMicrocontroller = "Send State\n"
    serialObject.write(commandForMicrocontroller.encode())

    index = 0
    while index < 7:
        lineReceived = serialObject.readline().decode('utf-8').strip()
        states.append(lineReceived)
        index += 1


# Configuring and starting the frame
def toggle_page(root, main_frame):
    # Create toggle frame widgets
    toggle_frame = ctk.CTkFrame(master=main_frame)
    toggle_frame.pack(fill="both", expand=True)

    # Add temperature label to the top right corner
    temperature_label = ctk.CTkLabel(master=toggle_frame, text=getTemperature(), font=("Roboto", 32))
    temperature_label.pack(anchor="ne", padx=25, pady=25)  # Use anchor="ne" to position in the top right corner
    # Update the temperature label every 3 seconds
    update_temperature_label(temperature_label, root)

    title_label = ctk.CTkLabel(master=toggle_frame, text="APPLIANCES", font=("Roboto", 24))
    title_label.pack(pady=12, padx=10)
    
    labels_and_buttons_for_toggle_page(toggle_frame, root)

    exit_button = ctk.CTkButton(master=toggle_frame, text="EXIT", command=root.quit)
    exit_button.pack(pady=12, padx=10)
    buttons.append(exit_button)


# Sets actions for keyboard navigation
def button_actions():
    for button in buttons:
        button.bind("<Up>", lambda event, button=button: navigate_up(event))
        button.bind("<Down>", lambda event, button=button: navigate_down(event))
        button.bind("<Return>", lambda event, button=button: button.invoke())
        

# Returns the temperature from the microcontroller
def getTemperature():
    commandForMicrocontroller = "Send Temperature\n"
    serialObject.write(commandForMicrocontroller.encode())

    lineReceived = serialObject.readline().decode('utf-8').strip()
    return lineReceived


# Update the temperature label every 3 seconds
def update_temperature_label(temperature_label, root):
    temperature_label.configure(text=getTemperature())
    root.after(3000, lambda: update_temperature_label(temperature_label, root))


# Configuration of labels and buttons
def labels_and_buttons_for_toggle_page(toggle_frame, root):
    # Create labels and buttons for each appliance
    for i in range(1, 8):
        appliance_frame = ctk.CTkFrame(master=toggle_frame)
        appliance_frame.pack(fill="both", pady=20, padx=750, expand=False)
        
        number_label = ctk.CTkLabel(master=appliance_frame, text=str(i), font=("Roboto", 24))  
        number_label.pack(side="left", padx=15, pady=5) 
        appliance_number_labels.append(number_label)

        state_label = ctk.CTkLabel(master=appliance_frame, text=getStateText(i-1), font=("Roboto", 24))
        state_label.pack(side="left", padx=15, pady=5) 
        appliance_state_labels.append(state_label)

        toggle_button = ctk.CTkButton(master=appliance_frame, text="TOGGLE", command=lambda i=i: toggle_appliance(appliance_state_labels[i-1], appliance_number_labels[i-1]), width=150,) 
        toggle_button.pack(side="right", padx=15, pady=5)
        buttons.append(toggle_button)

        if buttons:
            root.after(100, lambda: buttons[0].focus_set())
    
    # Create frame, label and button for all appliance toggleing
    all_appliances_frame = ctk.CTkFrame(master=toggle_frame)
    all_appliances_frame.pack(fill="both", pady=50, padx=750, expand=False)

    all_appliances_label = ctk.CTkLabel(master=all_appliances_frame, text="ALL", font=("Roboto", 24))
    all_appliances_label.pack(side="left", padx=15, pady=5)
    appliance_number_labels.append(all_appliances_label)  # Add the label to the list for focus navigation

    all_appliances_button = ctk.CTkButton(master=all_appliances_frame, text="TOGGLE", command=toggle_all, width=150)
    all_appliances_button.pack(side="right", padx=15, pady=5)
    buttons.append(all_appliances_button)  # Add the button to the list for focus navigation


# Returns a string representing the state of the appliance
def getStateText(index):
    appliance_elements = states[index].split() 
    if appliance_elements[3][6] == "0":
        return "OFF"
    return "ON"


# Toggles a particular appliance
def toggle_appliance(appliance_state, appliance_number):
    commandForMicrocontroller = "Change OutputAppliance\n"
    serialObject.write(commandForMicrocontroller.encode())

    if appliance_state.cget("text") == "ON":
        appliance_state.configure(text="OFF")
        state = "false"
        states_index_state = '0'
    else:
        appliance_state.configure(text="ON")
        state = "true"
        states_index_state = '1'
    
    if appliance_number.cget("text") == "1":
        pin_number = "0"
        states_index = 0
    elif appliance_number.cget("text") == "2":
        pin_number = "2"
        states_index = 1
    elif appliance_number.cget("text") == "3":
        pin_number = "5"
        states_index = 2
    elif appliance_number.cget("text") == "4":
        pin_number = "7"
        states_index = 3
    elif appliance_number.cget("text") == "5":
        pin_number = "10"
        states_index = 4
    elif appliance_number.cget("text") == "6":
        pin_number = "13"
        states_index = 5
    elif appliance_number.cget("text") == "7":
        pin_number = "15"
        states_index = 6

    appliance_data = "OutputElectricalAppliance, Pin:" + pin_number + ", Direction:true, State:" + state + ".\n"
    serialObject.write(appliance_data.encode())

    states[states_index] = "OutputElectricalAppliance, Pin:" + pin_number + ", Direction:1, State:" + states_index_state + "."


# Toggles all appliances
def toggle_all():
    for index in range (1,8):
        toggle_appliance(appliance_state_labels[index-1], appliance_number_labels[index-1])


# Event for UP arrow key
def navigate_up(event):
    global current_button_selection
    current_button_selection -= 1
    if current_button_selection < 0:
        current_button_selection = len(buttons) - 1
    buttons[current_button_selection].focus_set()


# Event for UP arrow key
def navigate_down(event):
    global current_button_selection
    current_button_selection += 1
    if current_button_selection >= len(buttons):
        current_button_selection = 0
    buttons[current_button_selection].focus_set()


# Receive commands from unicorn headset
def listenForUnicornCommands():
    serverAddressPort = ("127.0.0.1", 1000)
    bufferSize = 32768

    UDPClientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    UDPClientSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    UDPClientSocket.bind(serverAddressPort)

    controlCommands = ["UP", "DOWN", "SELECT"]

    #headset connection stuff
    global command
    while True:
        bytesAddressPair = UDPClientSocket.recvfrom(bufferSize)
        message = bytesAddressPair[0]
        for command in controlCommands:
            if str.encode(command) in message:
                if command == "UP":
                    navigate_up(None)
                elif command == "DOWN":
                    navigate_down(None)
                elif command == "SELECT":
                    buttons[current_button_selection].invoke()


def main():
    root = ctk.CTk()

    # Open thread for receiving commands from unicorn headset
    threadUDP = threading.Thread(target=listenForUnicornCommands)
    threadUDP.daemon = True
    threadUDP.start()

    start(root)
    serialObject.close()


if __name__ == "__main__":
    main()