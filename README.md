# UniCntrl (Универсальный контроллер либо Мультисенсор)
Универсальный контроллер:
- Прием и передача данных по интерфейсу CAN.
- Опрос оптического датчика (фотобарьера). Передача состояния оптического датчика в шину CAN согласно протоколу.
- Управление светодиодным табло согласно и звуковым индикатором согласно алгоритму.
- Передача данных в бортовой компьютер по протоколу MQTT.

В составе универсального контроллера:
- Процессорный модуль SK-iMX6ULL-NANO-2E
- Материнская плата для SK-iMX6ULL-NANO-2E
- Плата расширения и изоляции интерфейсов
- Оптический датчик GEZE GZ 470 V
- Светодиодное табло (бегущая строка на светодиодах, подключается по I2C/SPI)

Мультисенсор:
- Организовать опрос датчиков. Входят датчики температуры, влажности,CO2, опциональный выносной датчик температуры, датчик освещенности и управление продувом датчиков.
- Опрос USB зарядок по шине CAN.
- Передача данных (датчиков и зарядок) в бортовой компьютер по протоколу MQTT.

В составе мультисеносра:
- Процессорный модуль SK-iMX6ULL-NANO-2E
- Материнская плата для SK-iMX6ULL-NANO-2E
- Плата расширения и изоляции интерфейсов
- Датчик освещённости APDS9300
- Продуваемые датчики температуры, влажности SI7021 (GY-21) и CO2 MH-Z19C
- Дополнительный выносной датчик температуры DS18b20
- USB зарядки, подключаемые по интерфейсу CAN.

cd /UniCntrl/x86_utils

Формированияе адреса в дереве происходит через конфигурационный файл
- 1.Номер платы 
- 2.Номер секции в интерфейсе в которой находиться датчик
- 3.Интерфейс на котором висит датчик
- 4.Наименование параметра (температура влажность и.т.д)

Примеры:
Получение информации с sht21:
<https://code.woboq.org/linux/linux/drivers/hwmon/sht21.c.html>
 
     ./mosquitto_sub -h localhost -p 1883 -t 0/0/i2c-0/sht21/humidity -v
     ./mosquitto_sub -h localhost -p 1883 -t 0/0/i2c-0/sht21/temperature -v

Получение информации с apds9300:
<https://code.woboq.org/linux/linux/drivers/iio/light/apds9300.c.html>

     ./mosquitto_sub -h localhost -p 1883 -t 0/1/i2c-0/apds9300/in_illuminance -v

     ./mosquitto_sub -h localhost -p 1883 -t 0/1/i2c-0/apds9300/in_intensity0 -v
     ./mosquitto_sub -h localhost -p 1883 -t 0/1/i2c-0/apds9300/in_intensity1 -v

Получение информации с mhz19C:

     ./mosquitto_sub -h localhost -p 1883 -t 0/0/uart1/mhz19C/co2_ppm -v
     ./mosquitto_sub -h localhost -p 1883 -t 0/0/uart1/mhz19C/temperature -v

  Коллибровка имеет 3 варианта ZERO 0, SPAN 1, AUTO_ON 2, AUTO_OFF 3

     ./mosquitto_pub -h localhost -p 1883 -i myClient -t 0/0/uart1/mhz19C/setCalibrate -m "0"
     ./mosquitto_pub -h localhost -p 1883 -i myClient -t 0/0/uart1/mhz19C/setCalibrate -m "1"
     ./mosquitto_pub -h localhost -p 1883 -i myClient -t 0/0/uart1/mhz19C/setCalibrate -m "2"

  Коллибровка когда известно сколько в данный  момент ppm 

    ./mosquitto_pub -h localhost -p 1883 -t 0/0/uart1/mhz19C/setCalibrateSpan -m "1100" 

Получение информации с ds18b20:
<https://code.woboq.org/linux/linux/drivers/w1/slaves/w1_therm.c.html>

    ./mosquitto_sub -h localhost -p 1883 -t 0/0/o1wireIO1_24/ds18b20/temperature -v

Получение информации с usb зарядок:

    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb1_voltage -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb2_voltage -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb3_voltage -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb1_current -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb2_current -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb3_current -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb1_mistake -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb2_mistake -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/usb3_mistake -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/total_mistake -v
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/temperature -v

Получение usb id software зарядки

    ./mosquitto_pub -h localhost -p 1883 -t 0/0/can0/usb_charging/getIdUsbcharging -m "0"
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/id -v

Получение usb id hardware зарядки

    ./mosquitto_pub -h localhost -p 1883 -t 0/0/can0/usb_charging/getIdUsbcharging -m "1"
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/id -v
Получение usb id uniq зарядки

    ./mosquitto_pub -h localhost -p 1883 -t 0/0/can0/usb_charging/getIdUsbcharging -m "2"
    ./mosquitto_sub -h localhost -p 1883 -t 0/0/can0/usb_charging/id -v
