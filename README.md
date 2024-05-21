# Charge Communication Module

A device compatible with almost any LiPo battery under 48V, used to monitor and control charging remotely and provides warnings to a user for any faults.

# User Installation


## Hardware Setup

### TTN Device Registration

This will be instructions on how to register the device to the users own TTN

The Things Network Configuration

- Create an account
  - To begin, you need to create an accoung on [The Things Network(TTN)](https://www.thethingsnetwork.org/)
- Create an Application via TTN
  - Log in to the TTN Console and select your region for the cluster
  - Navigate to the Application tab and click on "Create application"
  - Fill in the Application ID, Application name and Description fields

- Register your device
  - Click the the "Register end device" button
  - Select the appropriate Frequency plan, LoRaWAN version, and Regional Parameter version
- Configure advanced settings
  - Set the Activation mode, Additional LoRaWAN class capabilities, Network defaults, Custer settings in the advanced setting section
- Generate DevEUI, Device Address, AppSKey, NwkSKey by clicking "Generate" button

Node-RED settings for TTN connection

- TTN Applications

  - Navigate MQTT tab under the Integration menu
  - Generate the Password in the Connection credential section

- Node-RED

  - Use the "mqtt in" node to receive data from the TTN server
  - Configure the server in the Porperties tab
    - Fill in the Server section in the Connection tab with the same public address from the TTN application 
    - Fill in the Username and Password in the Security tab with the same Connection credentials from the TTN application

  - Specify the Topic to subscribe to the desired massege topic from TTN server. Use "#" to receive messages from any topic

Integration into "The Things Stack Community Edition" aka "The Things Stack V3"
To use the ESP32-Paxcounter in The Things Stack Community Edition you need an account to reach the console. Go to:

- The Things Stack Community Edition Console
- choose your region and go to applications
- create an application by clicking "+ Add application" and give it a id, name, etc.
- create a device by clicking "+ Add end device"
- Select the end device: choose the Brand "Open Source Community Projects" and the Model "ESP32-Paxcounter", leave Hardware Version to "Unknown" and select your Firmware Version and Profile (Region)
-Enter registration data: choose the frequency plan (for EU choose the recommended), set the AppEUI (Fill with zeros), set the DeviceEUI (generate), set the AppKey (generate), choose a device ID and hit "Register end device"
- go to Applications -> "your App ID" -> Payload formatters -> Uplink, choose "Repository" and hit "Save changes"
The "Repository" payload decoder uses the packed format, explained below. If you want to use MyDevices from Cayenne you should use the Cayenne payload decoder instead.

## Software Installation

This project file is built using [PlatformIO IDE for embedded development](https://platformio.org/) and integrates into your chosen IDE, like VSCode or Atom.
You can also likely use Arduino IDE to build the project, but the instructions for this are not covered here.

### shared/lmic_config.h

If you would like to use a different country configuration, please see the next section on how to change this file. *This option is currently configured for Australia 915Hz*

### shared/loraconf.h

The file 'shared/loraconf.h' can be used to change the connection configuration.

To configure OTAA, leave `#define LORA_ABP` deactivated (commented). To use ABP, activate (uncomment) `#define LORA_ABP` in the file `shared/loraconf.h`.
The file `shared/loraconf_sample.h` contains more information about the values to provide.

*This is currently configured to use ABP, however you will need to update the appropriate TTN values from your setup above.

If using the OTAA option, you should also configure the 'shared/ota.conf' file.
By copying the template 'shared/ota_sample.conf' and inputting your WIFI netowork and key, you can use a local web browser to upload updates to the device wirelessly.
If you don't need wireless firmware updating capabilities simply rename 'ota_sample.conf' to 'ota.conf'

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

### Payload Configuration
This will contain instructions on how to add the payload masks to TTN

### Node-Red Configuration
This will contain instructions on how a user can add a device to the Node-Red platform

## CCM Uses


# Modifications

This lists the name and location of files that were modified for the purposes of this project, including comments on how they were adapted.

## Configuration & Connection

### platform.ini

This file was copied from 'platformio_orig.ini' as per the installation instructions for ESP32 CPU based boards.

### paxcounter.conf

This file was edited from `shared/paxcounter_orig.conf` and settings modified in this file according to your needs and use case. Please take care of the duty cycle regulations of the LoRaWAN network you're going to use. Copy or rename to `shared/paxcounter.conf`.

These settings were adjusted and tested to determine the most effective messaging frequency and contents.

### shared/lmic.config.h

This file is adapted to match user country and device hardware.

### shared/loraconf.h

Created from the template file in 'shared/loraconf_sample.h'

The configuration options for either the OTAA or ABP join method are detailed in the user instructions above.

## Sensor Configuration

The ESP32-Paxcounter framework supports up to 3 sensors, and has configurations already included for manufacturer reccomended options - including the BME280 Atmospheric Sensor we are using for temperature.
Additionally, it allows the configuration of custom sensors, which is what was used to create the voltage sensor and its interface with the relay.

To configure a custom sensor:
1. Necessary variables or libraries should be added
2. Sensor specific code should be added to the 'sensor_init' function in 'sensor.cpp'
3. Sensor specific code should be added to the 'sensor_read' function in 'sensor.cpp'
4. Optionally, payload functions should be included in 'payload.h' and 'payload.cpp'
5. The payload functions are then used in 'sensor.cpp' to send the appropriate deactivated

##

## ttgobeam10.h

Firstly, the sensor to be used must be enabled in either the 'paxcounter.conf' file, or the hal file for the board.
In this case the HAS_SENSOR_1 was updated in the board specific file.

For the BME280 Atmospheric sensor, the '#define HAS_BME 1' options and its additional variables and pins as specified in the manufacturers documentation were also defined.

## sensor.cpp

The sensors payload scheme was inserted into 'sensor.cpp'.

## payload.h



## payload.cpp 



# Credits

The framework for the TTGO T-Beam ESP32 communication protocol was derived or taken from the ESP32-Paxcounter of Oliver Brandmueller and Klaus Wilting 

Copyright 2018-2022 Oliver Brandmueller <ob@sysadm.in>
Copyright 2018-2022 Klaus Wilting <verkehrsrot@arcor.de>

Thanks to
- [LilyGo LoRa Series](https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series) for the TTGO T-Beam libraries and examples
- [CoreElectronics PiicoDev](https://github.com/CoreElectronics/CE-PiicoDev-BME280-MicroPython-Module) for the BME280 ATmospheric Sensor framework