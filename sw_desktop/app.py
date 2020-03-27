from Tkinter import *
import serial
from serial.tools.list_ports import comports

window = Tk()
window.title("bFunc Control")
window.geometry('500x200')

mainframe = Frame(window)
mainframe.grid(column=0,row=0, sticky=(N,W,E,S) )
mainframe.columnconfigure(0, weight = 1)
mainframe.rowconfigure(0, weight = 1)
mainframe.pack() #pady = 100, padx = 200)

# Create a Tkinter variable for Waveform
tkvar_wave = StringVar(window)
choices_wave = { 'sine', 'square', 'triangle' }
tkvar_wave.set('sine') # set the default option

# Create a Tkinter variable for serial ports
tkvar_port = StringVar(window)
choices_ports = [' ']
for port in comports():
    choices_ports.append(port[0])

choices_ports = set(choices_ports)
tkvar_port.set(' ') # set the default option

# Create a Tkinter variable for Mode Selection

# Create a Tkinter variable for Frequency Register 0
tkvar_freq0 = StringVar(window)
tkvar_freq0.set('10000')

# Create a Tkinter variable for Frequency Register 1
tkvar_freq1 = StringVar(window)
tkvar_freq1.set('10000')

# Create a Tkinter variable for Phase Register 0
tkvar_phase0 = StringVar(window)
tkvar_phase0.set('0')

# Create a Tkinter variable for Phase Register 1
tkvar_phase1 = StringVar(window)
tkvar_phase1.set('0')

# Create a Tkinter variable for Output Mode
choices_mode = [
    ("Waveform", "wave"),
    ("Modulation", "modulate")
]

tkvar_outmode = StringVar(window)
tkvar_outmode.set("wave")


# Initializes pyserial object
ser = serial.Serial()

# Debug Function to detect change in Waveform Dropdown
def change_wave_dropdown(*args):
    print( tkvar_wave.get() )

# Debug Function to detect change in Serial Port Dropdown
def change_port_dropdown(*args):
    newport = tkvar_port.get() 
    print( tkvar_port.get() )
    if newport != ' ':
        if ser.is_open:
            ser.close()
        ser.baudrate = 115200
        ser.port = newport
        ser.open()
        print ser

# Debug Function to detect change in radiobutton state
def change_outmode_radio(*args):
    new_mode = tkvar_outmode.get()
    if new_mode == "wave": # Disable freq1/phase1 buttons
        btn_freq1['state']  = "disabled"
        btn_phase1['state'] = "disabled"
    elif new_mode == "modulate": # Enable freq1/phase1 buttons
        btn_freq1['state']  = "normal"
        btn_phase1['state'] = "normal"
    print( new_mode )

# Helper Function to print contents of freq text box on button click
def clicked_freq0(*args):
    freq = txt_freq.get()
    if freq == '': # Add a check for numeric input
        print( "No freq input; not updated")
    else:
        tkvar_freq0.set(freq)
    sendstr = "freq0 {freq}\r\n"
    sendstr = sendstr.format(freq=freq)
    print(sendstr) 
    ser.write(sendstr.encode()) 

# Helper Function to print contents of freq text box on button click
def clicked_freq1(*args):
    freq = txt_freq.get()
    if freq == '': # Add a check for numeric input
        print( "No freq input; not updated")
    else:
        tkvar_freq1.set(freq)
    #print( "freq1 " + tkvar_freq1.get() )
    sendstr = "freq1 {freq}\r\n"
    sendstr = sendstr.format(freq=freq)
    print(sendstr) 
    ser.write(sendstr.encode()) 

# Helper Function to Set contents of phase0 variable on button click
def clicked_phase0(*args):
    phase = txt_phase.get()
    if phase == '':
        print( "No phase input; not updated")
    else:
        tkvar_phase0.set(phase)
    #print( "phase0 " + tkvar_phase0.get() )
    sendstr = "phase0 {phase}\r\n"
    sendstr = sendstr.format(phase=phase)
    print(sendstr) 
    ser.write(sendstr.encode()) 

# Helper Function to Set contents of phase1 variable on button click
def clicked_phase1(*args):
    phase = txt_phase.get()
    if phase == '':
        print( "No phase input; not updated")
    else:
        tkvar_phase1.set(phase)
    print( "phase1 " + tkvar_phase1.get() )
    sendstr = "phase1 {phase}\r\n"
    sendstr = sendstr.format(phase=phase)
    print(sendstr) 
    ser.write(sendstr.encode()) 

# Debug Function to print contents of waveform and freq:
def clicked_waveform(*args):
    #clicked_freq0()
    #clicked_phase0()
    freq = tkvar_freq0.get()
    wave = tkvar_wave.get()
    phas = tkvar_phase0.get()
    sendstr = "{wave} {freq} {phas}\r\n"
    sendstr = sendstr.format(wave=wave, freq=freq, phas=phas)
    print(sendstr) 
    ser.write(sendstr.encode()) 

### Row 1 ###
# Create a Label and a Dropdown Menu for Waveform Selection
wfm_label = Label(mainframe, text="Serial Port")
wfm_label.grid(row=1, column=1, sticky="w")

popupMenu_serial = OptionMenu(mainframe, tkvar_port, *choices_ports)
popupMenu_serial.grid(row=1, column=2, columnspan=3, sticky="ew")

tkvar_port.trace('w', change_port_dropdown)

### Row 2 ###
# Create a Label and a Dropdown Menu for Waveform Selection
wfm_label = Label(mainframe, text="Waveform")
wfm_label.grid(row=2, column=1, sticky="w")

popupMenu_wave = OptionMenu(mainframe, tkvar_wave, *choices_wave)
popupMenu_wave.grid(row=2, column=2, sticky="ew")

btn_func = Button(mainframe, 
                  text="Set Waveform", 
                  width=12,
                  command=clicked_waveform)
btn_func.grid(row=2, column=3, sticky="ew")

tkvar_wave.trace('w', change_wave_dropdown) # link function to change dropdown

### Row 3 ###
# Create a Label, Entry Box, and Button for frequency selection
freq_label = Label(mainframe, text="Frequency (Hz)")
freq_label.grid(row=3, column=1, sticky="w")

txt_freq = Entry(mainframe, width=10)
txt_freq.grid(row=3, column=2)

btn_freq0 = Button(mainframe, 
                   text="Set Freq0", 
                   width=10,
                   command=clicked_freq0)
btn_freq0.grid(row=3, column=3, sticky="ew")

btn_freq1 = Button(mainframe, 
                   text="Set Freq1", 
                   width=10,
                   command=clicked_freq1)
btn_freq1.grid(row=3, column=4, sticky="ew")
btn_freq1['state']  = "disabled" # Freq1 disabled in waveform mode

### Row 4 ###
# Create a Label, Entry Box, and Button for phase selection
phase_label = Label(mainframe, text="Phase (deg)")
phase_label.grid(row=4, column=1, sticky="w")

txt_phase = Entry(mainframe, width=10)
txt_phase.grid(row=4, column=2)

btn_phase0 = Button(mainframe, 
                    text="Set Phase0", 
                    width=10,
                    command=clicked_phase0)
btn_phase0.grid(row=4, column=3, sticky="ew")

btn_phase1 = Button(mainframe, 
                    text="Set Phase1", 
                    width=10,
                    command=clicked_phase1)
btn_phase1.grid(row=4, column=4, sticky="ew")
btn_phase1['state'] = "disabled" # Phase1 disabled in waveform mode

### Row 5 ###
# Create a Label and radio buttons for output mode selection
startrow = 6
phase_label = Label(mainframe, text="Output Mode")
phase_label.grid(row=startrow, column=1, sticky="w")
startrow += 1 
for text, mode in choices_mode:
    b = Radiobutton(mainframe,
                    text=text,
                    variable=tkvar_outmode,
                    value=mode)
    b.grid(row=startrow, column=1, sticky="w")
    startrow += 1

tkvar_outmode.trace('w', change_outmode_radio) # Link radiobuttons to change function

# Start Window Superloop
print(choices_wave)
print(choices_ports)
window.mainloop()
