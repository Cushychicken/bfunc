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

# Debug Function to detect change in Waveform Dropdown
def change_dropdown(*args):
    print( tkvar.get() )

# Debug Function to print contents of freq text box on button click
def clicked(*args):
    freq = txt_freq.get()
    if freq == '':
        print( "No freq input; not updated")
    else:
        tkvar_freq.set(freq)
    print( tkvar_freq.get() )

# Debug Function to print contents of waveform and freq:
def clicked_waveform(*args):
    freq = tkvar_freq.get()
    wave = tkvar.get()
    print( wave + " " + freq ) 

# Row 1
# Create a Label and a Dropdown Menu for Waveform Selection
wfm_label = Label(mainframe, text="Waveform")
wfm_label.grid(row=1, column=1, sticky="w")

popupMenu = OptionMenu(mainframe, tkvar, *choices)
popupMenu.grid(row=1, column=2, sticky="ew")

tkvar.trace('w', change_dropdown) # link function to change dropdown

# Row 2
# Create a Label, Entry Box, and Button for frequency selection
Label(mainframe, text="Frequency (Hz)").grid(row=2, column=1)

txt_freq = Entry(mainframe, width=10)
txt_freq.grid(row=2, column=2)

btn_freq = Button(mainframe, text="Set Freq0", width=10)
btn_freq.grid(row=2, column=3)
btn_freq.bind('<Button-1>', clicked)

# Row 3
# Create a Button for function selection
btn_func = Button(mainframe, text="Set Waveform", width=10)
btn_func.grid(row=3, column=2)
btn_func.bind('<Button-1>', clicked_waveform)

# Start Window Superloop
window.mainloop()
