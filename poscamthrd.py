import time
import argparse
from pmw3901 import PMW3901, PAA5100, BG_CS_FRONT_BCM, BG_CS_BACK_BCM

import threading
#import board
#import busio

#from smbus2 import SMBus


#import adafruit_icm20x

###!/usr/bin/env python
#import adafruit_bitbangio as bitbangio

#
#import microcontroller
print("""motion.py - Detect flow/motion in front of the PMW3901 sensor.

Press Ctrl+C to exit!
""")
# 
# parser = argparse.ArgumentParser()
# parser.add_argument('--board', type=str,
#                     choices=['pmw3901', 'paa5100'],
#                     required=True,
#                     help='Breakout type.')
# parser.add_argument('--rotation', type=int,
#                     default=0, choices=[0, 90, 180, 270],
#                     help='Rotation of sensor in degrees.')
# parser.add_argument('--spi-slot', type=str,
#                     default='front', choices=['front', 'back'],
#                     help='Breakout Garden SPI slot.')
# 
# args = parser.parse_args()

# Pick the right class for the specified breakout
SensorClass = PAA5100
#PMW3901 if args.board == 'pmw3901' else PAA5100

flo = SensorClass(spi_port=0, spi_cs=1, spi_cs_gpio=BG_CS_FRONT_BCM)
# if args.spi_slot == 'front' else BG_CS_BACK_BCM)
flo.set_rotation(0)
#args.rotation)

#i2c = bitbangio.I2C(board.D5,board.D4)#SMBus(3)#busio.I2C(board.D4,board.D5)  # uses board.SCL and board.SDA
#print(i2c.try_lock())
#print(i2c.scan())
print("in")
#icm = adafruit_icm20x.ICM20649(i2c)
print("..")

#lock = threading.Lock()

def main(args):
    print("welcome")
    

rx = 0
ry = 0
rz = 0

tx = 0
ty = 0
#icm.gyro_data_rate = 125
#icm.gyro_range = adafruit_icm20x.GyroRange.RANGE_500_DPS

# ~ def gyro():#thread for rotation readout
    # ~ global tx 
    # ~ global ty 

    # ~ global rx 
    # ~ global ry 
    # ~ global rz 
    # ~ #print(icm.readGyroRange)
    # ~ while True:
        # ~ x,y,z = icm.gyro
        # ~ with lock:
            # ~ rx=rx+x
            # ~ ry=ry+y
            # ~ rz=rz+z
            # ~ #flo.set_rotation(rz)
        # ~ #time.sleep(0.01)
        # ~ #print("Gyro X:%.2f, Y: %.2f, Z: %.2f rads/s" % (icm.gyro))
        
# ~ def posth(): #thread for position readout
    # ~ global tx 
    # ~ global ty 

    # ~ global rx 
    # ~ global ry 
    # ~ global rz 
    # ~ while True:
        # ~ try:
            # ~ x, y = flo.get_motion()
        # ~ except RuntimeError:
            # ~ continue
        # ~ with lock:
            # ~ tx += x
            # ~ ty += y
            # ~ ##TODO calculate winkel
        # ~ #print("Relative: x {:03d} y {:03d} | Absolute: x {:03d} y {:03d}".format(x, y, tx, ty))
        # ~ time.sleep(0.01)
        
# ~ def printth():
    # ~ global tx 
    # ~ global ty 

    # ~ global rx 
    # ~ global ry 
    # ~ global rz 
    # ~ while True:
        # ~ #with lock:
         # ~ #   rz+=1
        # ~ print("Rot: x {:.2f} y {:.2f} z {:.2f} | Pos: x {:03d} y {:03d}".format(rx/10, ry/10,ry/10, tx, ty))
        # ~ time.sleep(0.1)
        
        
        
def getpos(value):
    #print("getval")
    x = 0
    y = 0
    try:
        x, y = flo.get_motion()
    except RuntimeError:
        return 0,0
        #print("err")
    #print("retval")
    return x , y
    #except RuntimeError:
        #continue
    
    #tx += x
    #ty += y
    
        
#thrd1 = threading.Thread(target=gyro)
#thrd2 = threading.Thread(target=posth)
#thrd3 = threading.Thread(target=printth)



def destroy():
    print("bye")
    
#thrd3.start()
#thrd1.start()
#thrd2.start()


#thrd1.join()
#thrd2.join()


# ~ try:
    # ~ while True:
        # ~ print("..")
        # ~ print("Gyro X:%.2f, Y: %.2f, Z: %.2f rads/s" % (icm.gyro))
        # ~ try:
            # ~ x, y = flo.get_motion()
            
        # ~ except RuntimeError:
            # ~ continue
        # ~ tx += x
        # ~ ty += y
        # ~ print("Relative: x {:03d} y {:03d} | Absolute: x {:03d} y {:03d}".format(x, y, tx, ty))
        # ~ time.sleep(0.01)
# ~ except KeyboardInterrupt:
    # ~ pass
