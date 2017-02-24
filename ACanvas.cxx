#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <re2/re2.h>

#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>

#include <FL/fl_draw.H>

#include "ACanvas.h"

ACanvas::ACanvas(int x_, int y_, int w, int h, const char *label): 
    Fl_Double_Window(x_, y_, w, h, label)
{
	printf("%s()\n", __func__);
	flo = fl_create_offscreen(w, h);
	end();
}

/*****************************************************************************/
/* draw */
/*****************************************************************************/
void ACanvas::draw(void)
{
	printf("%s()\n", __func__);
	fl_copy_offscreen(0, 0, w(), h(), flo, 0, 0);
}

/*****************************************************************************/
/* manage incoming json draw commands */
/*****************************************************************************/

void ACanvas::processCommand(string json)
{
	printf("%s()\n", __func__);
	
	string m0, m1, m2, m3;
	fl_begin_offscreen(flo);

	while(!json.empty() && json[json.length()-1] == '\n') {
		json.erase(json.length()-1);
	}

	//printf("matching on: -%s-\n", json.c_str());

	if(RE2::FullMatch(json, "clear")) {
		fl_draw_box(FL_FLAT_BOX, 0, 0, w(), h(), fl_rgb_color(255, 255, 255));
	}
	else
	if(RE2::FullMatch(json, "box (\\d+) (\\d+)", &m0, &m1)) {
		int x = atoi(m0.c_str());
		int y = atoi(m1.c_str());
		fl_draw_box(FL_FLAT_BOX, x, y, 20, 20, fl_rgb_color(0, 255, 0));
	}

	fl_end_offscreen();
	redraw();
}

/*****************************************************************************/
/* handle fltk events */
/*****************************************************************************/
int ACanvas::handle(int event)
{
    int rc = 0; /* 0 if not used or understood, 1 if event was used and can be deleted */

	//printf("%s()\n", __func__);

	if(event == FL_RELEASE) {
		int mouseX = Fl::event_x();
		int mouseY = Fl::event_y();
		fl_begin_offscreen(flo);
		//printf("drawing box at %d,%d\n", mouseX, mouseY);
		fl_draw_box(FL_FLAT_BOX, mouseX, mouseY, 20, 20, fl_rgb_color(0, 0, 255));
		fl_end_offscreen();
		//printf("translated click (%d,%d) to (rank,file)=(%d,%d)\n",
		//	mouseX, mouseY, selRank, selFile);
		// with possible selection change, redraw()
		redraw();
	}

    return rc;
}


