
![Built for ESP32](https://img.shields.io/badge/Built%20for-ESP32-blue?logo=espressif&logoColor=white)

# Umgebungssensor Projekt
## ESP32-C3 (seeed studio)
### Pin Out zum Board
![ESP32-C3 Pin Out](/docs/pinout_c3.png)
Wichtige PINs für das Projekt:
- GPIO6: I²C SDA
- GPIO7: I²C SCK/SCL
- GND: Ground Potential
- 3V3: 3,3V Potential für BME280 und SH1106

### Vorbereitung zur Programmierung ([ArduinoIDE](https://www.arduino.cc/en/software/))
- Wenn du bereits ESP32 programmiert hast, kannst du diesen Punkt überspringen!
    - Hier bitte noch den passenden Treiber herrunterladen: [CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)
    - Im Board-Manager die ESP32 Boards installieren oder folgenden Link anführen: `https://dl.espressif.com/dl/package_esp32_index.json`
- Als Board zum Programmiern: XIAO_ESP32C3

> [!NOTE]
> In der IDE in der Menubar unter "Tools" → "USB CDC on Boot" → "Enable" einstellen. Somit sollte später keine Probleme auftreten, wenn du die Serielleschnittstelle benutzen möchtest.

Somit sollten alle Vorbereitung zur Programmierung getroffen worden sein.
## OLED-Display: SH1106
<img src="/docs/SH1106.jpg" alt="SH1106" width="250px"></img> \
Das OLED Display kann über das Bussystem **I²C** angesteuert werden. Es aggiert dabei als Slave. Mithilfe der Library [U8g2](https://github.com/olikraus/u8g2) wird die Anzeige angesteuert. Laut [Datasheet](https://cdn.shopify.com/s/files/1/1509/1638/files/1_3_Zoll_Display_Datenblatt_AZ-Delivery_Vertriebs_GmbH_rev.pdf?v=1606164520) kann es mit 3,3V oder 5V Versorgungsspannung bedient werden.<br/> <br/>
Weitere interessante Fakten:
- Dimensionen (LxBxT): 36 mm x 34 mm x 3 mm
- Auflösung: 128px x 64px
- Stromverbrauch: <11mA
- Displaygröße: 1,3 inch
- Displayfarbe: Monochrom (weiß)
## Sensor: BME280
<img src="/docs/BME280.jpg" alt="SH1106" width="250px"></img> \
Auch der Sensor wird via **I²C** angesteuert und befindet sich dabei ebenfalls im Slave-Modus. Die Softwareansteuerung erfolgte über die [Adafruit BME280 Library](https://github.com/adafruit/Adafruit_BME280_Library) die noch folgende Abhängigkeit mit der [Adafruit Unified Sensor Driver](https://github.com/adafruit/Adafruit_Sensor) hat. Auch dieses Modul kann mit 3,3V oder auch 5V betrieben werden, wie aus dem [Datenblatt](https://cdn.shopify.com/s/files/1/1509/1638/files/BME280_Barometrischer_Sensor_Modul_Datenblatt_AZ-Delivery_Vertriebs_GmbH.pdf?v=1608218816) zu entnehemen ist.<br/> <br/>
Weitere interessante Fakten:
- Temperature: -40°C bis 85 °C;  Genauigkeit: ±1.0°C
- Luftdruck: 300 hPa bis 1100 hPa; Genauigkeit: ±1hPa
- Luftfeuchtigkeit: 0% bis 100%; Genauigkeit: ±3%
- Stromverbrauch: <1mA
## Verdrahtung und PCB
### Verdrahtung
![Steckbrettaufbau](/docs/Steckplatine.png)
### PCB
![2D-View: PCB](/docs/pcb_2d.png)
![3D-View: PCB](/docs/pcb_3d.png)

## Gehäuse
In Autodesk Inventor wurde ein simples und kompaktes Gehäusedesign vorgenommen.
- [Deckel](/inventor/Deckel.ipt)
- [Boden](/inventor/Boden.ipt)

## Website
### Funktionen
- OTA Update
- Graphische Aufbereitung mit Chart
![Website](/docs/Website1.png)
![Website](/docs/Website2.png)

## Fertig
![Fertiger Aufbau](/docs/fertig.jpg)

