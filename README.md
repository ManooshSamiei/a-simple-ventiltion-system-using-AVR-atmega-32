# a-simple-ventiltion-system-using-AVR-atmega-32

In this project a simple ventilation system is simulated. Using 4 LM35 sensors, 3 of which are located inside and 1 outside the house, the inside and outside temperature is measured. The home temperature is calculated as an average of 3 sensors’ temperature.  The ventilation system is supposed to check the indoor temperature every 5 seconds and if the temperature has dropped from a predetermined temperature by 1 degree or more, the heating system will be turned on and if the temperature is one degree or more, higher than the predetermined temperature, It will be tuned off. Also, if the difference between the temperature of each of the sensors in the house was more than 2 degrees, the intensity of the output air (the fan) would increase from (1) to (2). All of the above information is displayed on an LCD as follows:

outside:3          Inside:2 

Fan:(1)            temp:23

“temp” is the temperature that is selected by the user. Using a 3x4 keypad the temperature is given to the system. Also it is possible to increase or decrease the temperature by one degree by pressing “+” and “-” buttons respectively.
