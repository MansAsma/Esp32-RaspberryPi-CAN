# Implementing CAN Bus with ESP32, Raspberry Pi 4, and MCP2515

This project demonstrates how to implement a CAN bus communication system between an ESP32 and a Raspberry Pi 4 Model B using MCP2515 CAN transceivers. 
The ESP32 reads temperature and humidity data from a DHT11 sensor and transmits the data as CAN frames to the Raspberry Pi. The Raspberry Pi, running Raspbian OS, receives and processes the CAN frames.

## Features
* Read temperature and humidity data using a DHT11 sensor connected to the ESP32.
* Transmit the sensor data as CAN frames using the MCP2515 transceiver.
* Receive and process CAN frames on the Raspberry Pi 4 Model B.
* Display the temperature and humidity values received by the Raspberry Pi.

## Hardware Used in this Project
   - ESP32 Board
   - Raspberry Pi 4 Model B (running Raspbian OS)
   - DHT11 Sensor
   - Two MCP2515 CAN Transceivers
   - Jumper WiresPower Supply
 
## Software Requirements

* ESP-IDF (for programming the ESP32) and Visual Studio Code (as the IDE for ESP32 development).
* Python-CAN (on Raspberry Pi to handle CAN communication) & SocketCAN (on Raspberry Pi for CAN interface) [ https://www.beyondlogic.org/adding-can-controller-area-network-to-the-raspberry-pi/]

## Wiring Diagram
![Esp32-MCP2515-RaspberryPi](https://github.com/user-attachments/assets/5d1a1c04-2c4e-426c-bd0d-4ab6d6337707)

# Implementation Steps

## 1. Setting Up the ESP32

   a. Install ESP-IDF:
       Follow the steps  [ https://medium.com/@mans.asma/esp-idf-using-vs-code-e996d1721bd1]
   
   b. Write the ESP32 Code
   
   * clone this project
   * Use the DHT sensor library from [https://github.com/jsuiker/esp-open-rtos/tree/3ba19d7c4e23b2ab6f7003b4ce9d572d79419540/extras/dht] to read temperature and humidity data from the DHT11 sensor. (code to be added under DHT11/Include/dht.h and source code  DHT11/src/dht.c)
   * Use the MCP2515 library to send CAN frames containing the sensor data [https://github.com/Microver-Electronics/mcp2515-esp32-idf] (source code to be added under MCP2515/src/mcp2515.c  and header files under MCP2515/Include/can.h ; MCP2515/Include/mcp2515.h)
   * Build and Flash the Code to ESP32

     ``` idf.py flash -p <PORT> ```

     ``` idf.py monitor -p <PORT> ```

## 2. Setting Up the Raspberry Pi

a. Enable SPI and Install Dependencies
  * Enable SPI using raspi-config:
   
     
     ``` sudo raspi-config ```
   
   * Install Python-CAN and configure SocketCAN:

    
     ```  sudo apt update ```
     
     
     ```sudo apt install python3-can```
     
     
     ```sudo apt install can-utils  ```

     
b. Configure the CAN Interface

  * Set up the CAN interface: (Set Can bitrate to 500KBPS)
      
    ``` sudo ip link set can0 up type can bitrate 500000 ```

  
  
  ## 3. Testing the System:
  
  * Power on the ESP32 and Raspberry Pi.

   ![setup](https://github.com/user-attachments/assets/9bdbdbee-4885-400a-9462-b33b44211966)

    
  * Run this command on the Raspberry Pi after setting up the CAN interface (monitors and displays all incoming CAN frames on the specified interface (can0 in this case) ):

    ``` candump can0 ```
  
  * Check the output for temperature and humidity values transmitted from the ESP32.

 **  ESP32 side : 

 
![esp32](https://github.com/user-attachments/assets/2e256134-93eb-4bf5-9f73-ef5fe3768396)


 ** RaspberryPi side:
 
![raspberryPi](https://github.com/user-attachments/assets/0b4234c5-b197-482a-8172-c3c65a9a4e34)




