# acanvas
Any program that can invoke a subprocess and write to its stdin can draw.

Draw commands are line oriented.

# internals
A thread monitors stdin for line input. Upon completing a line:

* uses FLTK's Fl::lock(); to pause the GUI loop
* instructs the canvas widget to process the line (eg: draw a box)
* drawing is done to offscreen buffer
* Fl::unlock() and Fl::awake() is called, resuming GUI loop and hinting at redraw
* draw() copies the offscreen buffer to onscreen buffer
