from Tkinter import *

window = Tk()
window.title("bFunc Control")
window.geometry('350x200')

mainframe = Frame(window)
mainframe.grid(column=0,row=0, sticky=(N,W,E,S) )
mainframe.columnconfigure(0, weight = 1)
mainframe.rowconfigure(0, weight = 1)
mainframe.pack() #pady = 100, padx = 200)

# Create a Tkinter variable for Waveform
tkvar = StringVar(window)
choices = { 'Sine', 'Square', 'Triangle' }
tkvar.set('Sine') # set the default option

# Create a Tkinter variable for Frequency
tkvar_freq = StringVar(window)
tkvar_freq.set('10000')

# Create a Tkinter variable for Frequency
tkvar_phase = StringVar(window)
tkvar_phase.set('0')

# Debug Function to detect change in Waveform Dropdown
def change_dropdown(*args):
    print( tkvar.get() )

# Debug Function to print contents of freq text box on button click
def clicked_freq(*args):
    freq = txt_freq.get()
    if freq == '':
        print( "No freq input; not updated")
    else:
        tkvar_freq.set(freq)
    print( tkvar_freq.get() )

# Debug Function to print contents of freq text box on button click
def clicked_phase(*args):
    phase = txt_phase.get()
    if phase == '':
        print( "No phase input; not updated")
    else:
        tkvar_phase.set(phase)
    print( tkvar_phase.get() )

# Debug Function to print contents of waveform and freq:
def clicked_waveform(*args):
    clicked_freq()
    clicked_phase()
    freq = tkvar_freq.get()
    wave = tkvar.get()
    phas = tkvar_phase.get()
    print( wave + " " + freq + " " + phas ) 

# Row 1
# Create a Label and a Dropdown Menu for Waveform Selection
wfm_label = Label(mainframe, text="Waveform")
wfm_label.grid(row=1, column=1, sticky="w")

popupMenu = OptionMenu(mainframe, tkvar, *choices)
popupMenu.grid(row=1, column=2, sticky="ew")

btn_func = Button(mainframe, text="Set Waveform", width=12)
btn_func.grid(row=1, column=3, sticky="ew")
btn_func.bind('<Button-1>', clicked_waveform)

tkvar.trace('w', change_dropdown) # link function to change dropdown

# Row 2
# Create a Label, Entry Box, and Button for frequency selection
freq_label = Label(mainframe, text="Frequency (Hz)")
freq_label.grid(row=2, column=1, sticky="w")

txt_freq = Entry(mainframe, width=10)
txt_freq.grid(row=2, column=2)

btn_freq = Button(mainframe, text="Set Freq0", width=10)
btn_freq.grid(row=2, column=3, sticky="ew")
btn_freq.bind('<Button-1>', clicked_freq)

# Row 3
# Create a Label, Entry Box, and Button for phase selection
phase_label = Label(mainframe, text="Phase (deg)")
phase_label.grid(row=3, column=1, sticky="w")

txt_phase = Entry(mainframe, width=10)
txt_phase.grid(row=3, column=2)

btn_phase = Button(mainframe, text="Set Phase0", width=10)
btn_phase.grid(row=3, column=3, sticky="ew")
btn_phase.bind('<Button-1>', clicked_phase)

# Start Window Superloop
window.mainloop()
