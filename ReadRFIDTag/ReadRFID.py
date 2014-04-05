import os
import serial
from serial.tools import list_ports
import sys

def serial_ports():

    if os.name == 'nt':
        # windows
        for i in range(256):
            try:
                s = serial.Serial(i)
                s.close()
                yield 'COM' + str(i + 1)
            except serial.SerialException:
                pass
    else:
        # unix
        for port in list_ports.comports():
            yield port[0]

def get_serial_port():
    """
    Returns the ACM (Arduino) port
    """
    ports = list(serial_ports())
    usbport = ""
    for port in ports:
        if "ACM" in port or "A601LQ0Y" in port:
            usbport = port
            print usbport
            # break
    if usbport == "":
        print "Could not find Arduino"
        sys.exit(1)
    return usbport

if __name__ == "__main__":
    # print list(serial_ports())
    print get_serial_port()

    ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
    ser2 = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    
    last = ser.read()

    while 1:
        # ser.write("1")
        current = ser.read(11)
        # print current
        if(current != last):
            # print current
            i = 0
            while(i < 500):
                ser2.write("o")
                i+=1
            
        

        last = current
    
    x = ser.read()          # read one byte
    s = ser.read(10)        # read up to ten bytes (timeout)
    line = ser.readline()   # read a '\n' terminated line
    ser.close()