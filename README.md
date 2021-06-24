# Sea-Wave-Measurement-And-Abnormality-Tracking-System
Arduino based virtual hardware simulator used to detect and analysis abnormality in sea waves in a water body using rule of twelfths

## Introduction

Basically, the project is all about calculating, computing and acknowledging the user about one of the unnoticed issues prevailing in our environment, that is the sea wave abnormality. Our project would estimate and compare the values of sea wave patterns with the expected values and gives out the nature of the sea waves height as output (whether the sea waves heights are Normal or Abnormal).  
The project was designed using Arduino UNO, ultra-sonic sensor and ESP8266. 
The expected sea waveheight of a particular water body at any time is calculated using a thumb rule called rule of twelfths. This project could be applied to all the water bodies that follow the rule
of twelfths pattern. The estimated sea wave height of any particular water body is derived from the Tidespy API (https://tidespy.com/client/RawApi.php).

### Rule of twelfths and the sea wave height

Most tides are semi-diurnal, meaning there are two high and two low tides a day 12:25 minutes apart. The ‘Rule of Twelfths’ is based on the assumption that the tide does not rise or fall at a constant rate throughout its duration. It also assumes the curve for the area is symmetrical.
The rule states that in the first hour after low tide the water level will rise by one twelfth of the range, in the second hour two twelfths, and so on according to the sequence  1:2:3:3:2:1.

![](https://eoceanic.com/images/tips/310/tidal_curve.jpg)


### API and API keys required

In order to work on this project you will be needing an api key from tidespy website https://tidespy.com/client/TideFrameSignup.php

*This api gives the tide information on any location it has.*

You will also be needing an time api key from this website https://ipgeolocation.io/ip-location-api.html.

*This api will give you the time information so that we can apply it in our rule of twelfths.*

### Project Screenshot



Proteus simulation



<img src="https://user-images.githubusercontent.com/67074796/123325471-ae693900-d555-11eb-8389-1a3ec08af7ed.PNG" width="800">


The NodeMCU (ESP8266) serial monitor output



<img src="https://user-images.githubusercontent.com/67074796/123325706-fdaf6980-d555-11eb-8c1d-14f799f0c77a.png" width="800">
