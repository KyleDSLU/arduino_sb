#! /usr/bin/env python

import serial
import time
import datetime
import struct

class Arduino():
    
    def __init__(self, port = None, baudrate = 115200, timeout = 1):
        
        self._start_message = b'\x02'
        self._end_message = b'\xff\xff'
        self._pkgtimeout = 10
        self._pkgtimeout_timer = 0
        self._messagewaittime = 0.003
        self._droppedpackages = 0

        self._serial_recursions = 0
        self._serial_timer = 300 
            
        if port:
            self.ser = serial.Serial(port = port, baudrate = baudrate, timeout = timeout)
        
        else:
            self.ser = None
            
    def initPort(self, port, baudrate, timeout = 1):
        
        if self.ser:
            self.ser.close()
        
        else:
            self.ser = serial.Serial(port = port, baudrate = baudrate, timeout = timeout)
            
    def closePort(self):
        
        if self.ser:
            self.ser.close()
            
    def get_analog_voltage(self,expected_values):
        
        if self.ser:
            self.ser.write(b'\x02')
            time.sleep(self._messagewaittime)
            data = self.ser.read(self.ser.inWaiting())
            parsedData = self._parseData(data)
            #print len(parsedData), len(data)
            try:
                if len(parsedData) != expected_values*2:
                    raise ValueError('Bad Serial Read')

                output = []
                for i in range(len(parsedData)/2):
                    output.append(struct.unpack('>h',parsedData[2*i:2*i+2])[0])
                #rawVoltage = struct.unpack('>h', parsedData)[0]
                self._pkgtimeout_timer = 0
                return output
                
            except:
                #print ('Bad Data recieved', data, parsedData)
                self._pkgtimeout_timer += 1
                self._droppedpackages += 1
                if self._pkgtimeout_timer > self._pkgtimeout:
                    raise ValueError ('Bad Data Timeout exceeded')
                else:    
                    self.ser.reset_input_buffer()
                    self.ser.reset_output_buffer()
                    return self.get_analog_voltage(expected_values)
            
        else:
            print ('Serial Communication not Established')
        
    def _parseData(self, package):
        start_message_length = len(self._start_message)
        end_message_length = len(self._end_message)
        start_index = 0
        end_index = len(package)-1
        pkg_length = len(package)
        
        START_FLAG  = False
        END_FLAG = False
        
        for i in range(end_message_length, pkg_length):
            if package[-i:pkg_length+end_message_length-i] == self._end_message:
                #print(package[-i:pkg_length+end_message_length-i], i)
                end_index = pkg_length - i
                END_FLAG = True
                break
        
        if END_FLAG:  
            self._serial_recursions = 0
            return package[len(self._start_message):end_index]

        if self._serial_recursions > self._serial_timer:
            return None
        
        else:
            self._serial_recursions += 1
            #print('Message Not Complete: Looping')
            #self._droppedpackages += 1
            package = package + self.ser.read(self.ser.inWaiting())
            return self._parseData(package)
                

if ( __name__ == "__main__" ):
    arduino = Arduino()


