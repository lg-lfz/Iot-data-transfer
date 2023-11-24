# Temperaturdaten über Wifi im Browser

## Hardware

siehe [Hardware](doc/hardware.md).

## Installation 

Verwendete Bibliotheken:

| Name | Link |
|------|------|
| ESPAsyncWebserver | https://github.com/me-no-dev/ESPAsyncWebServer?utm_source=platformio&utm_medium=piohome |

## Arduino-CLI

List connected boards:
```bash
arduino-cli board list
```

Output:
```bash
Port Protokoll Typ               Platinenname FQBN Kern
COM5 serial    Serial Port (USB) Unbekannt
```

Compile:
```bash
arduino-cli compile -b esp8266:esp8266:nodemcu SampleProject8266.ino
```
Output:
```bash
. Variables and constants in RAM (global, static), used 28348 / 80192 bytes (35%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ DATA     1504     initialized variables
╠══ RODATA   1068     constants
╚══ BSS      25776    zeroed variables
. Instruction RAM (IRAM_ATTR, ICACHE_RAM_ATTR), used 60331 / 65536 bytes (92%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ ICACHE   32768    reserved space for flash instruction cache
╚══ IRAM     27563    code in IRAM
. Code in flash (default, ICACHE_FLASH_ATTR), used 239936 / 1048576 bytes (22%)
║   SEGMENT  BYTES    DESCRIPTION
╚══ IROM     239936   code in flash

Benutzte Bibliothek Version Pfad

ESP8266WiFi         1.0     C:\Users\dieter\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\libraries\ESP8266WiFi
ESPAsyncTCP         1.2.4   C:\Users\dieter\Documents\Arduino\libraries\ESPAsyncTCP


Used platform   Version Pfad
esp8266:esp8266 3.1.2   C:\Users\dieter\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.1.2
```
Program:

```bash
arduino-cli upload -b esp8266:esp8266:nodemcu -p COM5 SampleProject8266.ino
```

```bash
esptool.py v3.0
Serial port COM5
Connecting....
Chip is ESP8266EX
Features: WiFi
Crystal is 26MHz
MAC: 48:3f:da:4b:c5:4d
Uploading stub...
Running stub...
Stub running...
Configuring flash size...
Auto-detected Flash size: 4MB
Compressed 274224 bytes to 201681...
Writing at 0x00000000... (7 %)
Writing at 0x00004000... (15 %)
Writing at 0x00008000... (23 %)
Writing at 0x0000c000... (30 %)
Writing at 0x00010000... (38 %)
Writing at 0x00014000... (46 %)
Writing at 0x00018000... (53 %)
Writing at 0x0001c000... (61 %)
Writing at 0x00020000... (69 %)
Writing at 0x00024000... (76 %)
Writing at 0x00028000... (84 %)
Writing at 0x0002c000... (92 %)
Writing at 0x00030000... (100 %)
Wrote 274224 bytes (201681 compressed) at 0x00000000 in 17.9 seconds (effective 122.5 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
New upload port: COM5 (serial)
```

## Pinout

### BME280

Der BME280 ist ein Temperatur-, Luftdruck- und Luftfeuchtigkeitssensor.

![BME280 Pinout - Temperature Humidity Barometric Pressure Sensor](assets/BME280-Pinout-Temperature-Humidity-Barometric-Pressure-Sensor.png)

### Arduino ESP8266

![Arduino ESP8266 Pinout](assets/ESP_Pinout.png)

### Verbindung

| BME280 | ESP8266 |
| ------ | ------- |
| VCC    | 3.3 V   |
| GND    | GND     |
| SCL    | D1      |
| SDA    | D2      |
