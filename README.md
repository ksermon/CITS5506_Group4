# Charge Communication Module

A device compatible with almost any LiPo battery under 48V, used to monitor and control charging remotely and provides warnings to a user for any faults.

# User Installation


## Hardware Setup

### TTN Device Registration

This will be instructions on how to register the device to the users own TTN

## Software Installation

This project file is built using [PlatformIO IDE for embedded development](https://platformio.org/) and integrates into your chosen IDE, like VSCode or Atom.
You can also likely use Arduino IDE to build the project, but the instructions for this are not covered here.

### shared/lmic_config.h

If you would like to use a different country configuration, please see the next section on how to change this file. *This option is currently configured for Australia 915Hz*

### shared/loraconf.h

The file 'shared/loraconf.h' can be used to change the connection configuration.

To configure OTAA, leave `#define LORA_ABP` deactivated (commented). To use ABP, activate (uncomment) `#define LORA_ABP` in the file `shared/loraconf.h`.
The file `shared/loraconf_sample.h` contains more information about the values to provide.

=== "Activate OTAA (Default), Deactivate ABP"
    ``` c linenums="18" title="shared/loraconf.h"
    --8<-- "shared/loraconf_sample.h:18:18"
    ```
=== "Deactivate OTAA, Activate ABP"
    ``` c linenums="18" title="shared/loraconf.h"
    #define LORA_ABP

*This is currently configured to use ABP, however you will need to update the appropriate TTN values from your setup above.

If using the OTAA option, you should also configure the 'shared/ota.conf' file.
By copying the template 'shared/ota_sample.conf' and inputting your WIFI netowork and key, you can use a local web browser to upload updates to the device wirelessly.
If you don't need wireless firmware updating capabilities simply rename 'ota_sample.conf' to 'ota.conf'

=== "Copy"
    ``` bash
    cp shared/ota_sample.conf shared/ota.conf
    ```
=== "Rename"
    ``` bash
    mv shared/ota_sample.conf shared/ota.conf
    ```

### Uploading

*These instructions taken directly from ESP32-Paxcounter Docs*

!!! warning

     1. After editing `paxcounter.conf`, use "clean" button before "build" in PlatformIO!
     2. Clear NVRAM of the board to delete previous stored runtime settings (`pio run -t erase`)

**by cable, via USB/UART interface:**

To upload the code via cable to your ESP32 board this needs to be switched from run to bootloader mode. Boards with USB bridge like Heltec and TTGO usually have an onboard logic which allows soft switching by the upload tool. In PlatformIO this happenes automatically.<p>
The LoPy/LoPy4/FiPy board needs to be set manually. See these
<A HREF="https://www.thethingsnetwork.org/labs/story/program-your-lopy-from-the-arduino-ide-using-lmic">instructions</A> how to do it. Don't forget to press on board reset button after switching between run and bootloader mode.<p>
The original Pycom firmware is not needed, so there is no need to update it before flashing Paxcounter. Just flash the compiled paxcounter binary (.elf file) on your LoPy/LoPy4/FiPy. If you later want to go back to the Pycom firmware, download the firmware from Pycom and flash it over.

**over the air (OTA), download via WiFi:**

After the ESP32 board is initially flashed and has joined a LoRaWAN network, the firmware can update itself by OTA. This process is kicked off by sending a remote control command (see below) via LoRaWAN to the board. The board then tries to connect via WiFi to a cloud service (<A HREF="https://github.com/paxexpress">PAX.express</A>), checks for update, and if available downloads the binary and reboots with it. If something goes wrong during this process, the board reboots back to the current version. Prerequisites for OTA are: 1. You own a PAX.express repository, 2. you pushed the update binary to your PAX.express repository, 3. internet access via encrypted (WPA2) WiFi is present at the board's site, 4. WiFi credentials were set in ota.conf and initially flashed to the board. Step 2 runs automated, just enter the credentials in ota.conf and set `upload_protocol = custom` in platformio.ini. Then press build and lean back watching platformio doing build and upload.

**over the air (OTA), upload via WiFi:**

If option *BOOTMENU* is defined in `paxcounter.conf`, the ESP32 board will try to connect to a known WiFi access point each time cold starting (after a power cycle or a reset), using the WiFi credentials given in `ota.conf`. Once connected to the WiFi it will fire up a simple webserver, providing a bootstrap menu waiting for a user interaction (pressing "START" button in menu). This process will time out after *BOOTDELAY* seconds, ensuring booting the device to runmode. Once a user interaction in bootstrap menu was detected, the timeout will be extended to *BOOTTIMEOUT* seconds. During this time a firmware upload can be performed manually by user, e.g. using a smartphone in tethering mode providing the firmware upload file.

## CCM Uses


# Modifications

This lists the name and location of files that were modified for the purposes of this project, including comments on how they were adapted.

## platform.ini

This file was copied from 'platformio_orig.ini' as per the installation instructions for ESP32 CPU based boards.
```ini linenums="6" title="Uncomment your board"
--8<-- "platformio_orig.ini:6:36"
```

=== "Copy"
    ``` bash
    cp platformio_orig.ini platformio.ini
    ```
=== "Rename"
    ``` bash
    mv platformio_orig.ini platformio.ini
    ```

## paxcounter.conf

This file was edited from `shared/paxcounter_orig.conf` and settings modified in this file according to your needs and use case. Please take care of the duty cycle regulations of the LoRaWAN network you're going to use. Copy or rename to `shared/paxcounter.conf`.

=== "Copy"
    ``` bash
    cp shared/paxcounter_orig.conf shared/paxcounter.conf
    ```
=== "Rename"
    ``` bash
    mv shared/paxcounter_orig.conf shared/paxcounter.conf
    ```

These settings were adjusted and tested to determine the most effective messaging frequency and contents.

```c linenums="135" title="national regulations in shared/lmic_config.h "
--8<-- "shared\paxcounter.conf:1:135"
```

## shared/lmic.config.h

This file is adapted to match user country and device hardware.

```c linenums="9" title="national regulations in shared/lmic_config.h "
--8<-- "shared/lmic_config.h:9:18"
```

## shared/loraconf.h

Created from the template file in 'shared/loraconf_sample.h'

=== "Copy"
    ``` bash
    cp shared/loraconf_sample.h shared/loraconf.h
    ```
=== "Rename"
    ``` bash
    mv shared/loraconf_sample.h shared/loraconf.h
    ```

The configuration options for either the OTAA or ABP join method are detailed in the user instructions above.

# Credits

The framework for the TTGO T-Beam ESP32 communication protocol was derived or taken from the ESP32-Paxcounter of Oliver Brandmueller and Klaus Wilting 

Copyright 2018-2022 Oliver Brandmueller <ob@sysadm.in>
Copyright 2018-2022 Klaus Wilting <verkehrsrot@arcor.de>

Thanks to
- [LilyGo LoRa Series](https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series) for the TTGO T-Beam libraries and examples
- [CoreElectronics PiicoDev](https://github.com/CoreElectronics/CE-PiicoDev-BME280-MicroPython-Module) for the BME280 ATmospheric Sensor framework