# lab-4-rom-based-temperature-conversion-group10


Temperature can be measured in Celsius or Fahrenheit scale. Let c and f be a temperature 
reading in Celsius and Fahrenheit scales. They are related by   
<p align="center">
𝑓 =9/5×𝑐+32   
</p>
The conversion involves multiplication and division operations and direct implementation 
requires a significant amount of hardware resource. For a simple application, such as a digital 
thermometer, we can create a lookup table for conversion and store it in a ROM. 
Consider a conversion circuit with following specification: 

- The range is between 0°C and 100°C (32°F and 212°F). 
- The input and output are in 8-bit unsigned format. 
- A separate format signal indicates whether the input is in Celsius or Fahrenheit scale. The 
output is to be converted to the other scale.    
We can create two lookup tables for the two conversions. Note that because of the small size of 
these tables, it is possible to store the two tables in a single BRAM module. Design the circuit 
and verify its operation. 
