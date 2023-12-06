# ESP32 Web Control Interface

This Arduino sketch sets up an ESP32 microcontroller to function as an Access Point (AP) with a web server. The web server hosts a control interface that allows users to interact with the following features:

    Toggle the state of LED1 and initiate a blinking mode.
    Toggle the state of LED2.
    Toggle the state of LED3.
    Adjust the brightness of LED4 using a range slider.
    Monitor the state of three buttons (BTNS2, BTNS3, BTNS4).

## Instructions:

    Upload the sketch to your ESP32 board using the Arduino IDE.
    Connect to the WiFi network named "ESP32_YilD" with the password "11223344".
    Open a web browser and navigate to the IP address 192.168.1.1 to access the control interface.

## Control Interface:

    LED1: Toggle ON/OFF and initiate blinking.
    LED2: Toggle ON/OFF.
    LED3: Toggle ON/OFF.
    LED4: Adjust brightness using the range slider.
    Button States: Monitor the states of BTNS2, BTNS3, and BTNS4.

### License: GNU GPL v3.0
    The GNU General Public License v3.0 (GPL-3.0) is a copyleft license that ensures software freedom. It grants users the right to use, modify, and share the software. 
    If you distribute derivative works, they must also be open source and under the GPL. This license promotes collaboration and protects users' freedom. 
    Feel Free to Credit me in your Project!
### Notes:

    Make sure to customize the WiFi credentials (SSID and password) according to your preferences.
    Adjust pin assignments and modify the HTML interface as needed for your hardware setup.
    Replace the placeholder license text with the actual license you choose for your project.
