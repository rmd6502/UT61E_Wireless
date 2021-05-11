# UT61E_Wireless
Wirelessly read a UT61E using ESP-Now.

The *meter* repository contains the code to read the meter's status updates. I connected it to the serial dongle provided with the meter, connecting pins 5 and 7 connected together to GND, pin 4 to +3V3, and pin _2_ to RxD (the schematic incorrectly shows pin 3 as TxD from the meter). This goes to an ESP32 DevkitC module, using pin 17 as RxD.

The *meter_receiver* repository is meant for a [Heltec Wifi Kit 32](https://heltec.org/project/wifi-kit-32/), and displays the received updates on the OLED.


Parsing code heavily based on https://github.com/4x1md/ut61e_py/blob/master/src/ut61e.py by @4x1md<br/>
MIT License<br/>
Copyright (c) 2017 Dmitry Melnichansky

ESP Now code heavily based on https://github.com/SensorsIot/Encrypted-Remote-Control by @SensorsIot (Andreas Speiss), no license provided in that repository.
