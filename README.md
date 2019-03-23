# Musicbox
*Nome provisório

Controlador de dispositivos ligados ao Spotify via tags RFID.

## Equipamento

ESP8266 NodeMCU v 1.0 [datasheet](https://raw.githubusercontent.com/jeffersonrpn/musicbox/master/docs/esp8266-nodemcu-pinout.png)
RFID-RC522 [pin layout](https://github.com/miguelbalboa/rfid#pin-layout)

## Recursos utilizados

Algumas configurações e páginas web do projeto são armazenadas no [sistema de arquivos](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html) do ESP8266.

## Dependências

* [ESP8266FS (v0.2.0)](https://github.com/esp8266/arduino-esp8266fs-plugin/releases/tag/0.2.0)
* [Arduino RFID Library for MFRC522 (v1.4.3)](https://github.com/miguelbalboa/rfid)
* [ArduinoJson (v5.x)](https://github.com/bblanchon/ArduinoJson)
