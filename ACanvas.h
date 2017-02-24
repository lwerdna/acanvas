#pragma once

#include <string>
#include <vector>
using namespace std;

#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Double_Window.H>

typedef void (*ChessBoard_callback)(void);

class ACanvas : public Fl_Double_Window {
	Fl_Offscreen flo;
	
	void draw(void);

    public:
    ACanvas(int X, int Y, int W, int H, const char *label=0);
 
	void processCommand(string json);

	/* event handler */
	int handle(int event);
};
