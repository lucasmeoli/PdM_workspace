# Final practice
## Author
  Lucas Pablo Meoli

## Description
The aim of the project is to shift the position of an LED through an 8x32 LED matrix by moving an 
accelerometer. Every time the user moves it, an internal register of the accelerometer is activated, 
allowing to determine that there is a new coordinate reading available. From these coordinates, the 
value must be decoded to establish a correlation between the obtained coordinate and the position of 
a lit LED within the matrix. Once this is done, the LED is turned on at the corresponding position, 
and the coordinate value is displayed via UART. On the other hand, the accelerometer has 4 different
 measurement sensitivities. To select the desired sensitivity, a button will be used. If the 
 button is pressed twice in a row, the sensitivity of the accelerometer will decrease; whereas if 
 it is pressed only once, the sensitivity will increase.