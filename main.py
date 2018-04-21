import serial
import time
import datetime

class Arduino():
    
    def __init__(self, port = None, baudrate = 203400, timeout = 1):
        
        self._start_message = b'\xff\xff'
        self._end_message = b'\xff\xff'
        self._pkgtimeout = 10
        self._pkgtimeout_timer = 0
        self._messagewaittime = 0.0
        self._droppedpackages = 0
            
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
            
    def getRawPosition(self):
        
        if self.ser:
            self.ser.write(b'\x02')
            time.sleep(self._messagewaittime)
            data = self.ser.read(self.ser.inWaiting())
            parsedData = self._parseData(data)
            #print(data, parsedData)
            try:
                rawPosition = (parsedData[1] << 8) | parsedData[2]
                self._pkgtimeout_timer = 0
                
                return rawPosition
            except:
                print ('Bad Data recieved', data, parsedData)
                self._pkgtimeout_timer += 1
                self._droppedpackages += 1
                if self._pkgtimeout_timer > self._pkgtimeout:
                    raise ValueError ('Bad Data Timeout exceeded')
                else:    
                    return self.getRawPosition()
            
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
#            for i in range(pkg_length-end_message_length):
#                if package[i:i+start_message_length] == self._start_message:
#                    start_index = i+2
#                    START_FLAG = True
#                    break
            return package[:end_index]
        
        else:
            #print('Message Not Complete: Looping')
            #self._droppedpackages += 1
            package = package + self.ser.read(self.ser.inWaiting())
            return self._parseData(package)
                

if ( __name__ == "__main__" ):
    arduino = Arduino()


