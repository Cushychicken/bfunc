import serial
import platform

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

try:
    port = ''
    if platform.system() == 'Linux':
        print("linux!")
        port = '/dev/ttyACM0'
    elif platform.system() == 'Darwin':
        port = '/dev/tty.usbmodem00000000001A1'

    ser = serial.Serial(port,
                        baudrate=115200)
except Exception as inst:
    print(type(inst))
    print(inst.args)
    print(inst)
    exit("Serial device not found. Exiting.")

input(startup_help)
ser.write('sine 500 0\r\n'.encode())
input('1) Scope should register a sine wave of 500Hz as output. `sine 500 0`\n')
ser.write('freq0 1000\r\n'.encode())
input('2) Scope should register frequency output at 1kHz.    `freq0 1000`\n')
ser.write('phase0 180\r\n'.encode())
input('3) Sine phase should be inverted by 180 degrees.    `phase0 180`\n')
ser.write('triangle\r\n'.encode())
input('4) Output waveform should now be triangle.    `triangle`  \n')
ser.write('square\r\n'.encode())
input('5) Output waveform should now be square.      `square`    \n')

