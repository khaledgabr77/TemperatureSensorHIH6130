/*
 * Copyright (c) 2021, Micropolis
 * All rights reserved.
 
 * Created on Sun Aug 23 10:30:17 2021
 * @author: Khaled Gabr
 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 *  TEMPERATURE ROSSERIAL Node PUBLISHER test app
 *
 *  Copyright 2021  Micropolis Team
 *  http://www.micropolis.ae
 * 
 */
    
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
