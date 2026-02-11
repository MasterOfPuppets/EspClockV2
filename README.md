# EspClockV2

A smart WiFi clock built with ESP32 and MAX72XX LED matrix, featuring web configuration, NTP sync, and automatic timezone detection.

## Features
*   **Time Synchronization:** Precise NTP timekeeping with global servers.
*   **Smart Timezone:** Automatic detection via GeoIP (IP-API) with POSIX rule support.
*   **Web Interface:** Configure alarms, brightness, and timezone via browser (`http://clock-XXXX.local`).
*   **Alarm:** Set specific alarm times and days of the week.
*   **Animations:** Various visual effects when minutes change (Scroll, Dissolve, Jump, etc.).
*   **WiFi Manager:** Captive portal for WiFi network configuration without code recompilation.
*   **Hardware Control:** Support for physical button and buzzer for audible alarms.

## Required Hardware
*   ESP32 Board
*   MAX72XX LED Matrix Module (4 devices chain, FC16 type)
*   Passive/Piezo Buzzer
*   Pushbutton

## Pinout (Connections)

| Component | ESP32 Pin | Notes |
| :--- | :--- | :--- |
| **MAX72XX CLK** | GPIO 4 | Clock |
| **MAX72XX DIN** | GPIO 6 | Data In |
| **MAX72XX CS** | GPIO 7 | Chip Select |
| **Button** | GPIO 3 | Internal Pull-up |
| **Buzzer** | GPIO 2 | PWM |

## How to Use
1.  **First Boot:** The clock creates a WiFi Access Point named `clock-XXXX`.
2.  **Configuration:** Connect to this network and go to `192.168.4.1` (or wait for the captive portal) to set up your WiFi credentials.
3.  **Web Access:** Once connected, the clock displays its IP and mDNS name (e.g., `http://clock-AB12.local`). Use this address to change settings.
4.  **Physical Button:**
    *   **Short Press:** Stops the alarm or scrolls the device name/IP.
    *   **Factory Reset:** Hold while powering on the ESP32 to clear WiFi settings.

## Required Libraries
*   MD_Parola
*   MD_MAX72XX
*   WiFiManager (tzapu)
