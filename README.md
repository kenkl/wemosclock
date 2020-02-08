# wemosclock

A simple demo of the [HiLetgo 0.96" ESP32 OLED](https://www.amazon.com/HiLetgo-ESP-WROOM-32-Bluetooth-Development-Display/dp/B072HBW53G/) (Wemos) dev board running an NTP-sourced clock. Based on [ESP32_NTPClock](https://github.com/kenkl/ESP32_NTPClock), this does what it does - source the time, based on timezone/DST configured herein, and display it continuously.

Included is the [ThingPulse SSD1306](https://github.com/ThingPulse/esp8266-oled-ssd1306) libraries to drive the on-board OLED.

Gratuitious action snapshot:

![wemosclock](https://i.imgur.com/qUzxkLA.jpg "wemosclock")

