# SerialComm

C++ library designed to run on an Arduino-like microcontroller to facilitate data communication with a host computer running Python.

The associated Python libary for the host computer is in the following module in a different repository:
 - [serial_comm](https://github.com/billtubbs/sercommpy/tree/main/serial_comm)

## System for sending and receiving data between a PC and an Arduino

The method of transmitting and receiving data is based on a demonstration script published on the Arduino forum in 2014 by user Robin2:
 - [Demo of PC-Arduino comms using Python](https://forum.arduino.cc/t/demo-of-pc-arduino-comms-using-python/219184) (Mar 2014)

which I updated and translated into Python 3 compatible code here:
 - [robin2_demo/ComArduino_py2.py](robin2_demo/ComArduino_py2.py) - updated Python 2 script
 - [robin2_demo/ComArduino.py](robin2_demo/ComArduino.py) - modified for Python 3+

Three 'special' byte values are defined:

```cpp
START_MARKER = 254
END_MARKER = 255
SPECIAL_BYTE = 253
```

The process of sending data from a PC to the Arduino or vice-versa involves the following steps:
 1. Prefacing the message with the START_MARKER byte
 2. Encoding the data by replacing all instances of byte values 253, 254 or 255 with a pair of
    bytes, (SPECIAL_BYTE, 0), (SPECIAL_BYTE, 1) or (SPECIAL_BYTE, 2) accordingly
 4. Terminating the message with the END_MARKER.

The process of receiving data involves:
 1. Reading and discarding data until the START_MARKER is encountered
 2. Decoding the data by replacing all pairs of bytes, (SPECIAL_BYTE, 0), (SPECIAL_BYTE, 1) or
    (SPECIAL_BYTE, 2) with their original values 253, 254 or 255
 3. Stopping when the END_MARKER is encountered.

Note that the amount of data sent may be slightly longer (depending on how many times 253, 254, 
or 255 occur) but the receiving computer can clearly identify the start and end of the data.
