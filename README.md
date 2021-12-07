# UniCntrl
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
