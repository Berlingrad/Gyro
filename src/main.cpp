#include <mbed.h>
#include "l3gd20_driver.h"
#include "math.h"
#define PI 3.1415926





int main() {
    
     /**
    SPI spi(PF_9, PF_8, PF_7); // mosi, miso, sclk
    DigitalOut cs(PC_1);
    
    // Chip must be deselected
    cs = 1;
 
    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(8,3);
    spi.frequency(1000000);
 
    while (1) {
    
      // Select the device by seting chip select low
      cs = 0;
  
      // Send 0x8f, the command to read the WHOAMI register
      spi.write(0x8f);
  
      // Send a dummy byte to receive the contents of the WHOAMI register
      int whoami = spi.write(0x00);
      printf("WHOAMI register = 0x%X\n", whoami);
  
      // Deselect the device
      cs = 1;

      wait_us(1000000);
      
    }
    **/
    
    float degree2radian, degree, radian_angle, displacement, radius, timer;
    int counter;
    float sum;
    float array[40];
   
    SPI spi(PF_9, PF_8, PF_7); // mosi, miso, sclk
    L3GD20Gyroscope gyroscope(&spi, PC_1);
    

    int err = gyroscope.init();
    if (err) {
          MBED_ERROR(MBED_ERROR_INITIALIZATION_FAILED, "Gyroscope initialization failed");
    }

    float w_dps[3];
    counter = 0;
    radius = 0.4; 
    sum = 0;
    timer = 0;
    

    while (1) {

        
        
        gyroscope.read_data_dps(w_dps);
        float x = w_dps[0];
        float y = w_dps[1];
        float z = w_dps[2];
        //printf("wx: %+7.2f dps, wy: %+7.2f dps, wz: %+7.2f dps\n",  x, y, z);

        degree2radian = PI / 180;
        degree = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        displacement = radius * sin(degree * 0.0174533);
        
        array[counter] = (displacement > 0.1) ? displacement : 0;

        sum += array[counter];

        counter ++;
        timer ++;

        if (counter >= 39) {
          counter = 0;
        }

        
        
        

        printf("Time: %+7.1fs, wx: %+7.2f dps, wy: %+7.2f dps, wz: %+7.2f dps, cumulative distance:%+7.2fm, step length: %+7.2fm\n", timer * 0.5, x, y, z, sum, displacement);
     
        ThisThread::sleep_for(500ms);
 
    

        

    }
    
    
    
}