/*
 ============================================================================
 Name        : sht21.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
SHT21 is Temperature and Humidity sensor. This device will communicate over I2C bus to the microcontroller.
This device 0x40 has I2C slave address.

This driver either we can compile as static or as a module. I will prefer to compile as a module.

I interfaced the SHT21 over I2C-1 bus on BeagleBone Black

Loading the SHT21 driver:
echo sht21 0x40 > /sys/bus/i2c/devices/i2c-1/new_device

By this command the i2c client device will be added to the I2C master controller. We can check kernel log messages for
the checking.

Also we can see the loaded driver (sht21) by using lsmod.

The device read valvues will be exported through the sysfs. We can read the exported temp & humididity raw values and we can apply the caliberation equation upon that.

cat /sys/bus/i2c/devices/1-0040/hwmon/hwmon0/temp1_input
cat /sys/bus/i2c/devices/1-0040/hwmon/hwmon0/humidity1_input

Caliberation for Temperature:
T = -46.85 + 175.72 * (Temp_raw)/(2^16)

Caliberation for Humidity:
H = -6 + 125 * (RelativeHumidity_raw)/(2^16)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_BUF 64

int main(int argc, char *argv)
{
  FILE *fp;
  char path[MAX_BUF];

  //Open a file to I2C Bus 3 and tell it that there is a BMP085 located at 0x77.
  snprintf(path,sizeof(path), "/sys/bus/i2c/devices/i2c-0/new_device");
  if((fp = fopen(path,"w")) == NULL) {
  printf("file open failed\n");
  return 1;
  }

   rewind(fp);
   fprintf(fp, "sht21 0x40\n");
   fflush(fp);
   fclose(fp);

   while(1) {
     sleep(2);
     char buf[MAX_BUF];
     //Attempt to open the file of the device.
     snprintf(path, sizeof path, "/sys/bus/i2c/devices/0-0040/hwmon/hwmon1/temp1_input");
     if((fp = fopen(path, "r")) == NULL) {
        printf("cannot open device file\n");
        return 1;
   }

   //Attempt to read the temperature from the device.
   if((fgets(buf, MAX_BUF, fp)) == NULL){
   printf("cannot read device\n");
   }
   fclose(fp);

   float temp = atoi(buf);
   printf("result: %f & Temperature: %f\n",temp, ((175.72 * (temp / 65536)) - 46.85));
   //Attempt to open the file of the device.
   snprintf(path, sizeof path, "/sys/bus/i2c/devices/0-0040/hwmon/hwmon1/humidity1_input");
   if((fp = fopen(path, "r")) == NULL) {
   printf("cannot read\n");
   return 1;
   }

   //Attempt to read the pressure from the device.
   if((fgets(buf, MAX_BUF, fp)) == NULL){
   printf("cannot read\n");
   }
   fclose(fp);

   float humidity = atoi(buf);
    printf("Humidity: %f\n\n", (125 * (humidity / 65536) - 6));
  }
}
