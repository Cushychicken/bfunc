# bfunc - An open source, breadboard friendly function generator

These are the source files for bfunc, a function generator I've been desiging, building, and programming in my spare time. 

The fastest way to understand what's going on here is to read [the design document](http://cushychicken.github.io/bfunc-design-doc/). That should give you a good top-level understanding of this project. If it doesn't, there's a Contact link on my blog. I encourage you to let me know if there's something you don't understand, or is otherwise unclear!

## Project Structure

All hardware files can be found in the `hardware/` directory. All hardware design was done using KiCAD. 

Software is broken into two directories: 

* `sw_embedded/` for all the code running on the bFunc board, and
* `sw_desktop/`, which contains the Python app for the GUI. 

The sw_embedded directory contains two subfolders:

* `stm32_cube_bfunc/` contains all the software used for building the prototype, which I made out of a STM32 BluePill board and a SparkFun Minigen Pro shield. 
* `bfunc_rev1/` contains all the software used in the first board revision. 

## How can I contribute? 

I'm so glad you asked! Check out the [Call for Users](http://cushychicken.github.io/bfunc-call-for-users/) page on my blog. That will outline the next steps for getting a board, and getting yourself set up to work on it. 

After all that, if you _still_ can't get enough bFunc in your life - I've been keeping weekly project journals on my blog in addition to the design document. Here is a [link to the first one](http://cushychicken.github.io/bfunc-weekone-log/) to get you started.  
