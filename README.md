# M5CARDPUTER-Phisher
English:
This is a fork from https://github.com/n0xa/m5stick-nemo/, version 2.5.0. 
This started as a fork to understand how the code works...
The only modification is the change in the NEMO Portal (renamed for WiFiPhisher on menu and WiFi-Portal network name) for a generic router firmware update to get WiFi creds instead Google account.
The code is not finished. I plan to remove the first field in portal.h, the ones that ask for Network SSID confirmation, as it sounds odd and non leggit.
I don't know how to do it yet. Helps are very welcome. 

Right  now thigs are like this:

Portugues:
Esse e um fork do https://github.com/n0xa/m5stick-nemo/, versao 2.5.0.
Ele teve como intuito inicial a minha compreensao de como o codigo funciona...
A unica modificacao foi a mudanca do NEMO Portal (renomeado para WiFiPhisher no menu e WiFi-Portal a rede WiFi) para um portal que simula uma pagina generica de uma atualizacao de Roteador para capturar credenciais WiFi ao inves de contas Google.
O codigo nao esta terminado ainda. Eu planejo remover o primeiro campo no arquivo portal.h, que pede a confirmacao do SSID da Rede, ja que aparenta nao ser legitimo.
Eu ainda nao sei como fazer isso. Ajudas sempre serao bem vindas.

Ate o momento esta assim:

![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher1.jpeg)

Example of captive portal

Exemplo de portal


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher2.jpeg)

Custom captive portal.

Portal customizado.


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher3.jpeg)

How it looks on iPhone.

Como aparenta no iPhone.


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher4.jpeg)

Connecting to custom WiFi Network...

Conetando a rede customizada...


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher5.png)

Captive portal appearence.

Aparencia do portal.


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher6.png)

When filled fields.

Quando os campos estao preenchidos.


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher7.png)

After enter credentials.

Depois de inserir as credenciais.


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher8.jpeg)

Cardputer return info (it plays a bip sound). It can be changed in portal.h. Comment line 290 with // at the beginning.

Aviso no Cardputer (ele emite um sinal sonoro). Pode ser modificado no portal.h. Comente a linha 290 com // no inicio.


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher9.png)

Captured credentials in 172.0.0.1/creds, also recorded in the sdcard...

Credenciais capturadas em 172.0.0.1/creds, e tambem gravadas no cartao SD...


![m5phisher picture](https://github.com/arm-ARMY/m5phisher/blob/main/m5phisher10.png)

172.0.0.1/ssid link appearence.

Aparencia do link 172.0.0.1/ssid.


All other features works as expected (Clone Portal, Deauth Attack and Deauth+Clone),  redirecting to WifiPhisher Portal. All Spam Funny is in Portuguese, and maybe I change it in the future. But you can edit wifispam.h to add the originals in n0xa repository or your own. In the build directory there is the .bin image in Portuguese. I'll compile one in English.

Todos os outros recursos funcionam como esperado (Clonar Portal, Ataque Deauth e Deauth+Clone), e direcionam para o WiFiPhisher Portal. Todas as redes de Spam Engracado estao em portugues. Peguei elas de uma lista que achei na internet, penso em mudar alguns nomes. Sugestoes sempre serao bem vindas. A imagem .bin no diretorio build esta em Portugues. Vou compilar a versao em Ingles tambem.

All the code was made by n0xa and cotributors. Thanks for uploading this! By now all information in this main page remains with all the original project instructions. 

Todo o codigo foi desenvolvido por n0xa e contribuidores. Obrigado por publicar esse codigo! A partir daqui a pagina principal segue com as intrucoes do projeto original.

# M5Stick-NEMO

Firmware for high-tech pranks on M5Stack ESP32 Devices

![M5-Nemo Matrix Logo](https://github.com/n0xa/m5stick-nemo/blob/main/NEMOMatrix.png)
Logo by @unagironin

## Name and Background
NEMO started a personal project to help me learn more about ESP32 development with the Arduino IDE. I decided to replicate a few common, trending pranks that were getting a lot of attention in the tech community, as a challenge to myself, and to also better understand these attacks.  
NEMO is named after the small, clever and stubborn fish in Finding Nemo. This project stands in contrast to another high-tech gadget that's associated with certain sea-dwelling creatures. I did want to prove that there are a lot of things you can do with a small development kit and some curiosity. I have no delusions of superseding the capabilities of any similar device with this project. It's just for fun, and my own education.  

![M5-Nemo on M5StickC family and M5Cardputer](https://github.com/n0xa/m5stick-nemo/blob/main/M5-Nemo.jpg)

## My Changelog
* Adjusted .github/workflow/compile.yml to compile with the DEAUTH function.
* Added suport to SONG on StickC Plus and Plus2
* Added an "Attack Menu" when seeing details of an Access Point
  - Clone Portal (Creates an Evil portal with the same SSID of the target)
  - Deauth attack (broadcasts deauthentication frames to devices connected to this Access Point)
  - Deauth+Clone (Creates the evil portal with the same SSID and sends deauthentication frames to that AP)
* Added option in Settings to Mount/Unmount SDCard when using M5StickC devices
* Created new file to handle SPI/SD if not using CARDPUTER
* SDCARD is now working on M5StickC Plus2

## My ToDo list
* ideas?

## Features
* [TV B-Gone](http://www.righto.com/2010/11/improved-arduino-tv-b-gone.html) port (thanks to MrArm's [HAKRWATCH](https://github.com/MrARM/hakrwatch)) to shut off many infrared-controlled TVs, projectors and other devices
* [AppleJuice](https://github.com/ECTO-1A/AppleJuice) iOS Bluetooth device pairing spam
* Bluetooth device notification spamming for SwiftPair (Windows) and Android
* WiFi Spam - Funny SSIDs, WiFi Rickrolling, and a Random mode that creates hundreds of randomly-named SSIDs per minute
* WiFi NEMO Portal - A captive portal that tries to social engineer email credentials - saves usernames and passwords to SD Card (if inserted into a supported reader)
* WiFi SSID Scanner - Display 2.4 GHz SSIDs nearby, get information about them, and even clone the SSIDs in NEMO Portal
* User-adjustable 24 Hour digital clock backed by the M5 Stick RTC so it holds relatively stable time even in deep sleep and low battery mode
* EEPROM-backed Settings for rotation, brightness, automatic dimming and NEMO Portal SSID
* Battery level and credits in settings menu

## User Interface
There are three main controls:
* Home - Stops the current process and returns you to the menu from almost anywhere in NEMO
* Next - Moves the cursor to the next menu option. In function modes, this usually stops the process and returns you to the previous menu.
* Select - Activates the currently-selected menu option, and wakes up the dimmed screen in function modes  

* StickC and StickC-Plus
  * Power: Long-press the power button for 6 seconds to turn off the unit
  * Home: Tap the power button (closest to the USB port)
  * Next: Tap the side button
  * Select: Tap the M5 button on the front of the unit  

* Cardputer
  * Home: Tap the Esc/~/` key or the Left-Arrow/, key
  * Next/Prev: Tap the Down-Arrow/. key and Up-Arrow/; keys to navigate
  * Select: Tap the OK/Enter key or Right-Arrow/? key  

## NEMO Portal
In NEMO Portal mode, NEMO activates an open WiFi Hotspot named "Nemo Free WiFi" (configurable in portal.h) with DNS, DHCP and Web servers activated. 
* NEMO Portal serves a fake login page that claims to provide internet access if you log in.
* This is a social engineering attack, and will log the username and passwords entered on the page. 
* From the Wifi Scan details, you can clone an existing SSID from the scan list. Exiting NEMO Portal will clear the Evil Twin SSID
* You can view captured credentials by connecting to the portal from your own device and browsing to http://172.0.0.1/creds
* You can set a custom SSID by connecting to the portal from your own device and browsing to http://172.0.0.1/ssid
* If your device supports EEPROM for settings, the custom SSID you enter will be saved as the default, even if powered off.
* If your device has an SD Card reader with a FAT filesystem formatted card inserted, the usernames and passwords will be logged to nemo-portal-creds.txt on the SD Card for you to peruse later. 
* SD Card support is only enabled by default on the M5Stack Cardputer platform. It can be enabled on M5Stick devices but an SD Card reader must be built and attached to the front panel pin header.
* NEMO Portal is only for use on professional engagements with a valid scope of work, educational or demonstration purposes. Storage, sale, or use of personal information without consent is against the law. 🤓

## How to compile and burn m5phisher

* Install ESP-IDF tools per the [Espressif Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
* Open the esp-idf CMD tool (on Windows) - on Mac or Linux, esp-idf.py and esptool.py should be in the system path.
* esptool.py --port COMPORT -b 115200 write_flash -z 0x0 m5phisher.bin
  * port may be a COM port e.g. COM4, COM11 on Windows. On Mac and Linux it will usually be in /dev such as /dev/ttyUSB0, /dev/ttyACM0 or /dev/cu.usbserial-3

## Building from Source (with Arduino CLI)

- Install Arduino CLI
- Add M5Stack Index to Arduino Core
- Add M5Stack Libraries

```bash

# Install m5stack boards
arduino-cli core install m5stack:esp32  --additional-urls https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json --log-level warn --verbose

# Install required library
arduino-cli lib install M5Cardputer --log-level warn --verbose
arduino-cli lib install IRRemoteESP8266 --log-level warn --verbose

# Git clone repo
git clone https://github.com/arm-ARMY/m5phisher.git
cd m5phisher

# Compile sketch
arduino-cli compile --fqbn m5stack:esp32:m5stack_cardputer -e --build-property build.partitions=huge_app --build-property upload.maximum_size=3145728 ./m5phisher.ino --log-level warn --verbose

```

- This will create multiple binaries based on partition sketch, you can merge a single binary using `esptool``
- Install esptool - `pip install -U esptool`

```bash

esptool.py --chip esp32s3 merge_bin --output final.bin 0x0000 m5phisher.ino.bootloader.bin 0x8000 m5phisher.ino.partitions.bin 0x10000 m5phisher.ino.bin
```

- You can now flash the merged binary firmware using `esptool`

```bash

esptool.py write_flash -z 0 final.bin
```


## Troubleshooting
* Several features output debugging information to the serial monitor. Use the Serial Monitor feature in Arduino IDE or M5Burner to gather this information. It may have useful hints. When filing a bug report, it often helps to include serial monitor output.
* Reset the EEPROM. On models with EEPROM settings support, use "Clear Settings" from the settings menu, or hold the "Next" button (Side key on StickC models, Tab or Down Arrow on Cardputer) while powering on. 
* TV-B-Gone's IR LED can be observed through a smart phone camera, emitting a pale purple beam of light. If it seems to be on constantly, or if it never flashes at all during TV-B-Gone operations, something is wrong. Report a bug. There's a known issue with TVBG not working after using Bluetooth spam or random wifi spam.
* Try viewing wifi lists from several different devices if you suspect wifi spam isn't working. Sometimes, Linux network manager can see networks that smart phones cannot. Please include the results of this testing if reporting wifi spam problems.
* Apple has patched a lot of Bluetooth stuff since summer 2023. If testing AppleJuice, try some of the AppleTV device types, as they tend to be more reliable due to apple not filtering out weaker bluetooth signals for that platform.


## Reporting Bugs
Please report bugs via GitHub Issues. These are easier to track than comments on social media posts, M5Burner entries, etc. If something isn't working, please include:
* Firmware version
* How you installed it (M5Burner, compiled yourself, esptool.py)
* Hardware 
* Specifically what feature and options are not working
* How you determined that it is not working and what testing you did. Include device models and operating systems of what you tested with, and any errors or relevant output from Serial Monitor, if applicable.
* If you figure out how to fix a bug you identified, PRs are welcome!

## Contributing
Contributions are welcome. 
* Pliease look at the GitHub Issues for the project. There are feature suggestions and bugs reported there, and I'd appreciate PRs that address those.
* When submitting a Pull Request, please target the develop branch. The easiest way to do this is to fork ALL branches, or to simply create a "develop" branch in your own fork, then use GitHub to Sync your develop branch.
* Take note of how certain hardware (like the LED and RTC) are defined and gated in the code and try to stick to those patterns. Also, use the definitions for FGCOLOR, BGCOLOR, TEXT_SIZE* and the DISP alias when outputting things to the built-in display.
* Feel free to add your github to the contributors array as part of your pull request.

Things I'd like help on:
* Improved localization/translations of the menu, not just NEMO Portal HTML.
  * Probably needs a new .h file
  * Be configurable in settings and use an eeprom byte to save the setting
  * Make gratuitous use of `const` in the implementation so that localization strings are stored only in, and referenced directly from, the flash storage as opposed to using lots of SRAM.
* Infrared
  * A new "Region" of TV-B-Gone that's full of additional IR codes that can turn RGB LED strips, air conditioners, fans, sound bars and the like on and off
  * A way to convert or use flipper zero IR codes from within NEMO's code-base
* Figure out a way to read and display battery level on the Cardputer and StickC-Plus2 models that lack an AXP192 PMU. The [battery code in the Cardputer factory demo](https://github.com/m5stack/M5Cardputer-UserDemo/tree/main/main/hal/bat) might be a good place to start.
* TARGETED Deauthentication against a specific access point only. Deauth spamming PRs will be rejected.

Things I probably won't merge in:
* Bulk wifi deauthentication spamming
* Bluetooth spam that potentially disrupts health and fitness trackers, smart watches, etc.
