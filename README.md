# UniCntrl (Универсальный контроллер либо Мультисенсор)
- Прием и передача данных по интерфейсу CAN.
- Опрос оптического датчика (фотобарьера). Передача состояния оптического датчика в шину CAN согласно протоколу.
- Управление светодиодным табло согласно и звуковым индикатором согласно алгоритму, описанному в техническом задании.
- Передача данных в бортовой компьютер по протоколу MQTT. По сути часть параметров, полученных по CAN нужно передать в бортовой компьютер. В обратную сторону нужно быть подписчиком событий, и передавать их в шину CAN. Формат и состав передаваемых данных требует согласования. Бортовой компьютер и универсальны контроллер находятся в локальной сети транспортного средства.

В составе универсального контроллера:
- Процессорный модуль SK-iMX6ULL-NANO-2E
- Материнская плата для SK-iMX6ULL-NANO-2E
- Плата расширения и изоляции интерфейсов
- Оптический датчик GEZE GZ 470 V
- Светодиодное табло (бегущая строка на светодиодах, подключается по I2C/SPI)

Мультисенсор:
- Организовать опрос датчиков. Входят датчики температуры, влажности,CO2, опциональный выносной датчик температуры, датчик освещенности и управление продувом датчиков.
- Опрос USB зарядок по шине CAN.
- Передача данных (датчиков и зарядок) в бортовой компьютер по протоколу MQTT. Формат и состав передаваемых данных определяется и описывается подрядчиком. Бортовой компьютер и мультисенсор находятся в локальной сети транспортного средства.

В составе мультисеносра:
- Процессорный модуль SK-iMX6ULL-NANO-2E
- Материнская плата для SK-iMX6ULL-NANO-2E
- Плата расширения и изоляции интерфейсов
- Датчик освещённости APDS9300
- Продуваемые датчики температуры, влажности SI7021 (GY-21) и CO2 MH-Z19C
- Дополнительный выносной датчик температуры DS18b20
- USB зарядки, подключаемые по интерфейсу CAN.

cd /UniCntrl/x86_utils

     ./mosquitto_sub -h localhost -p 1883 -t 0/0/i2c-0/sht21/humidity -v
     ./mosquitto_sub -h localhost -p 1883 -t 0/0/i2c-0/sht21/temperature -v

     ./mosquitto_sub -h localhost -p 1883 -t 0/1/i2c-0/apds9300/in_illuminance -v
     ./mosquitto_sub -h localhost -p 1883 -t 0/1/i2c-0/apds9300/in_intensity0 -v
     ./mosquitto_sub -h localhost -p 1883 -t 0/1/i2c-0/apds9300/in_intensity1 -v

     ./mosquitto_sub -h localhost -p 1883 -t 0/0/uart1/mhz19C/co2_ppm -v
     ./mosquitto_sub -h localhost -p 1883 -t 0/0/uart1/mhz19C/temperature -v

  Коллибровка имеет 3 варианта ZERO 0, SPAN 1, AUTO_ON 2, AUTO_OFF 3

     ./mosquitto_pub -h localhost -p 1883 -i myClient -t 0/0/uart1/mhz19C/setCalibrate -m "0"
     ./mosquitto_pub -h localhost -p 1883 -i myClient -t 0/0/uart1/mhz19C/setCalibrate -m "1"
     ./mosquitto_pub -h localhost -p 1883 -i myClient -t 0/0/uart1/mhz19C/setCalibrate -m "2"

  Коллибровка когда известно сколько в данный  момент ppm 

    ./mosquitto_pub -h localhost -p 1883 -t 0/0/uart1/mhz19C/setCalibrateSpan -m "1100" 

    ./mosquitto_sub -h localhost -p 1883 -t 0/0/o1wireIO1_24/ds18b20/temperature -v

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
