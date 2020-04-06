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


raw_input(startup_help)
raw_input('1) Set a sine wave of 500Hz as output. `sine 500 0`\n')
raw_input('2) Change frequency output to 1kHz.    `freq0 1000`\n')
raw_input('3) Invert the phaes by 180 degrees.    `phase0 180`\n')
raw_input('4) Change the waveform to triangle.    `triangle`  \n')
raw_input('5) Change the waveform to square.      `square`    \n')

