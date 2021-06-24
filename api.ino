
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>

ESP8266WiFiMulti WiFiMulti;

WiFiClient client;

                                                                      
                                                                      //connection towifi
void setup() {

  Serial.begin(9600);
  //connection to wifi
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("ZTE-EdUSdD", "tj5kjkq4");
}

String ab;
void loop() {
   String a;
  if ((WiFiMulti.run() == WL_CONNECTED)) {
                                                                                              // tide and time api extraction;
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    HTTPClient https2;
    
    if (https.begin(*client, "https://tidespy.com/api/tideturns?pn=1341&unit=m&start=20210605&days=3&key=y78I5sZQtg6YEPlGV0zSr2nJbOUBLoh1")) {  // HTTPS
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String tide_data = https.getString();
      String curr_time;
      //Serial.println(tide_data);
      delay(1000);
      if (Serial.available()){
       a = Serial.readStringUntil('\r');
       if (https2.begin(*client, "https://api.ipgeolocation.io/timezone?apiKey=88b4d540f7e4413baf2bae276058c37d")){
        int httpCode = https2.GET();
         if (httpCode > 0){
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
            curr_time = https2.getString();
          }
         }
         else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
       }
       else {
      Serial.printf("[HTTPS] Unable to connect\n");
        }
        int wave_height = a.toInt();
                                                                                                    // wave nature detecting function call
        checkAbnormality(wave_height, tide_data, curr_time, a);
      }
      else{
        //Serial.println("Not available");
      }
       }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
}

void checkAbnormality(int wh, String td, String ct, String actual){
  
       float th = actual.toFloat();
       Serial.println((String)"Actual Wave height: " + th + "m");
       
       
       DynamicJsonDocument doc(2048);
       DeserializationError error = deserializeJson(doc, td);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }

        String ReqStart = doc["ReqStart"]; // "20210511"
        const char* Name = doc["Name"]; // "South Fork"
        const char* PlaceUnit = doc["PlaceUnit"]; // "ft"
        int len = 0;
        for (JsonObject elem : doc["Turns"].as<JsonArray>()) {
        len++;
        }
         DynamicJsonDocument doc2(2048); 
        DeserializationError error2 = deserializeJson(doc2, ct);
        
        if (error2) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error2.f_str());
          return;
        }
        
       ;

        const char* timezone = doc2["timezone"]; // "Asia/Kolkata"
        float timezone_offset = doc2["timezone_offset"]; // 5.5
        String date = doc2["date"]; // "2021-05-12"
        String date_time = doc2["date_time"]; // "2021-05-12 20:03:19"

                                                                                              // coversion time to minutes
        int hr = date_time.substring(11,13).toInt();
        int min = date_time.substring(14,16).toInt();
        int tot_min = hr * 60 + min;
        Serial.println((String)"time now: " + tot_min + "min");  
        int i = 0;
        int m1, m2;
        float h1, h2;
        int pos;
                                                                                                   //Extracting the high tide and low tide value between the range in which the current time falls 
        for (JsonObject elem2 : doc["Turns"].as<JsonArray>()) {
        String ApiDate = elem2["Date"]; // "20210511", "20210511", "20210511", "20210512", "20210512", ...
             if (ApiDate.toInt() == ReqStart.toInt() + 1){ // 25th

             Serial.println((String)"Todays date: " + ApiDate);
             
             int flag = 0;
             for ( pos = i; pos < len - 4; pos++){
              String min = doc["Turns"][pos]["Minute"];
              if(min.toInt() > tot_min){
                 String min2 = doc["Turns"][pos]["Minute"];
                 String min1 = doc["Turns"][pos-1]["Minute"];
                 m1 = min1.toInt();
                 m2 = min2.toInt();
                String height1 = doc["Turns"][pos-1]["Height"];
                String height2 = doc["Turns"][pos]["Height"]; 
              h1 = height1.toFloat();
              h2 = height2.toFloat();
                 flag = 1;
                 break;
              }
             }
             if(flag == 0){
              String min2 = doc["Turns"][pos]["Minute"];
             String min1 = doc["Turns"][pos-1]["Minute"];
             m1 = min1.toInt();
             m2 = min2.toInt();
              String height1 = doc["Turns"][pos-1]["Height"];
              String height2 = doc["Turns"][pos]["Height"];  
              h1 = height1.toFloat();
              h2 = height2.toFloat();
             }
             break;
          
        }
        i++;
        }
        Serial.println((String) "Time interval between which low tide or high tide occur:" + m1 + "min " + m2 + "min");
        Serial.println((String)"Height of low tide and high tide: " + h1 + "m " + h2 + "m");

                                                                                                                //rule of twelfths
        int diff;
        if (m1 < m2){
          diff = m2-m1; //approx 6:30min
        }
        else{
          diff = (1440 - m1) + m2;
        }
        Serial.println((String)"Difference in time interval " + diff + "min");
         Serial.println("On applying Rule of twelfths...");
        int interval = diff/6;
        Serial.println((String)"Time interval gap " + interval + "min");
       
        int lb = m1;
        int row = 0;
        if (tot_min > m1){
          while(tot_min > lb ){
            lb+=interval;
            row++;
          }
        }
        else{
          row = ((1440-m1) / interval) + 1;
          lb = m1 + (row * interval);
          Serial.println(lb);
          while(tot_min > (lb % 1440)){
            lb += interval;
            row++;
          }
        }
        Serial.println((String)"The tide happens at " + row + " row");
        float hdiff = h2-h1;
        //Serial.println(hdiff);
        float hinterval = hdiff / 12;
        //Serial.println(hinterval);
        float sl, el;
        if(row >= 1){
          sl = h1;
          el = sl + hinterval;
        }
        if (row >= 2){
          sl = el;
          el = sl+hinterval*2;
        }
        if(row>=3){
          sl = el;
          el = sl+hinterval*3;
        }
        if(row >= 4){
          sl = el;
          el = sl+hinterval*3; 
        }
        if(row >= 5){
          sl = el;
          el = sl+hinterval*2;
        }
        if(row >= 6){
          sl = el;
          el = sl+hinterval*1;
        }
        Serial.println((String)"At this time the height of the time should be between " + sl + "m and " + el + "m");
                                                                                                //comparison of actual height with predicted height and display the result
  if ((sl <= th && th <=el) || (sl >= th && th >=el)){
     Serial.println((String) th +  " is within the range. Hence, The wave height is Normal");
}
else{
  Serial.println((String) th +  "m is not within the range. Hence, The wave height is Abnormal");
}
}
