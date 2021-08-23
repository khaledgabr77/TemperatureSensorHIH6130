// HIH_6130_1  - Arduino
// Kahled Gabr 
// SCL ( 21) ------- SCL 
// SDA ( 20) ------- SDA 
//
// Note 2.2K pullups to 5 VDC on both SDA and SCL
//
// 5V ----------------- Vdd
// GND ---------------- GND 
//

    
#include <Wire.h>
#include <ros.h>
#include <std_msgs/Float32.h>


//Set up the ros node and publisher
std_msgs::Float32 temp_msg;
ros::Publisher pub_temp("temperature", &temp_msg);

std_msgs::Float32 hum_msg;
ros::Publisher pub_hum("hummity", &hum_msg);
ros::NodeHandle nh;
int sensorAddress = 0x27 >> 1;  // From datasheet sensor address is 0x91


void setup(void)
{  
  Wire.begin();

  
  nh.getHardware()->setBaud(9600);


  nh.initNode();
  nh.advertise(pub_temp);
  nh.advertise(pub_hum);


}
long publisher_timer;

    
void loop(void)
{

   int temperature;
   unsigned int H_dat, T_dat;
   float RH, T_C;
   

    byte  Hum_H, Hum_L, Temp_H, Temp_L;
    sensorAddress = 0x27;
      Wire.beginTransmission(sensorAddress); 
      Wire.endTransmission();
      delay(100);
    
      if (millis() > publisher_timer) {
        Wire.requestFrom((int)sensorAddress, (int) 4);
        
        if(4 <= Wire.available()){
       
          Hum_H = Wire.read();
          Hum_L = Wire.read();
          Temp_H = Wire.read();
          Temp_L = Wire.read();
      
          H_dat = (((unsigned int)Hum_H) << 8) | Hum_L;
          T_dat = (((unsigned int)Temp_H) << 8) | Temp_L;
          T_dat = T_dat / 4;

     
           RH = (float) H_dat * 6.10e-3;
           T_C = (float) T_dat * 1.007e-2 - 40.0;
           temp_msg.data = T_C;
           hum_msg.data = RH;

           pub_temp.publish(&temp_msg);
           pub_hum.publish(&hum_msg);
        }
      publisher_timer = millis + 1000;
      }
      nh.spinOnce();
      

     delay(1000);


     
      
   
}
