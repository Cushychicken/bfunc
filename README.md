# bfunc - An open source, breadboard friendly function generator

These are the source files for bfunc, a function generator I've been desiging, building, and programming in my spare time. 

All hardware files can be found in the hardware directory. All hardware design was done using KiCAD. 

All software files are thus far located in the sw_embedded/ directory. I'll eventually be adding a sw_desktop/ directory for a light desktop GUI that can be used to control the board via USB Virtual Com Port. 

The sw_embedded directory contains two subfolders:

* stm32_cube_bfunc/ contains all the software used for building the prototype, which I made out of a STM32 BluePill board and a SparkFun Minigen Pro shield. 
* bfunc_rev1/ contains all the software used in the first board revision. 

For a good top-level understanding of this project, I encourage you to mosey on over to my blog and read [the design document](http://cushychicken.github.io/bfunc-design-doc/). 

If you can't get enough beyond that - I've been keeping weekly project journals on my blog in addition to the design document. Here is a [link to the first one](http://cushychicken.github.io/bfunc-weekone-log/) to get you started.  
