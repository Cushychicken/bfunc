import serial

startup_help = """
This is the Outgoing QA Script for bFunc.
The objective is to manually run every instruction in order, 
and validate correct behavior of a bFunc board.

You will need a 1kHZ oscillator to use as a phase 
reference. This is built into most oscilloscopes for probe 
compensation.

Hit Ctrl-C to exit script upon QA failure. 

Hit Enter upon successful completion of a step to proceed.

Hit Enter to begin.

"""

ser = serial.Serial()

try:
    ser = serial.Serial('/dev/tty.usbmodem00000000001A1',
                         baudrate=115200)
except:
    exit("Serial device not found. Exiting. Check for /dev/tty.usbmodem00000000001A1.")

raw_input(startup_help)
ser.write('sine 500 0\r\n')
raw_input('1) Set a sine wave of 500Hz as output. `sine 500 0`\n')
ser.write('freq0 1000\r\n')
raw_input('2) Change frequency output to 1kHz.    `freq0 1000`\n')
ser.write('phase0 180\r\n')
raw_input('3) Invert the phase by 180 degrees.    `phase0 180`\n')
ser.write('triangle\r\n')
raw_input('4) Change the waveform to triangle.    `triangle`  \n')
ser.write('square\r\n')
raw_input('5) Change the waveform to square.      `square`    \n')

