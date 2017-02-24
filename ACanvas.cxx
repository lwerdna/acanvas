/* cstdlib */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* c++ stuff */
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/* google re2 stuff */
#include <re2/re2.h>

/* fltk stuff */
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
/* afltk stuff */
#include <afltk/ChessBoard.h>

/* our local stuff */
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
	//printf("%s()\n", __func__);

	fl_copy_offscreen(0, 0, w(), h(), flo, 0, 0);
}

/*****************************************************************************/
/* manage incoming json draw commands */
/*****************************************************************************/

void ACanvas::processCommand(string json)
{
	//printf("%s()\n", __func__);
	
	string m0, m1, m2, m3;

	while(!json.empty() && json[json.length()-1] == '\n') {
		json.erase(json.length()-1);
	}

	//printf("matching on: -%s-\n", json.c_str());

	if(RE2::FullMatch(json, "clear")) {
		fl_begin_offscreen(flo);
		fl_draw_box(FL_FLAT_BOX, 0, 0, w(), h(), fl_rgb_color(255, 255, 255));
		fl_end_offscreen();
	}
	else
	if(RE2::FullMatch(json, "resize (\\d+) (\\d+) (\\d+) (\\d+)", &m0, &m1, &m2, &m3)) {
		int x = atoi(m0.c_str());
		int y = atoi(m1.c_str());
		int w = atoi(m2.c_str());
		int h = atoi(m3.c_str());
		/* actually resize */
		resize(x, y, w, h);
		/* make new offscreen buffer */
		fl_delete_offscreen(flo);
		flo = fl_create_offscreen(w, h);
	}
	else
	if(RE2::FullMatch(json, "box (\\d+) (\\d+)", &m0, &m1)) {
		int x = atoi(m0.c_str());
		int y = atoi(m1.c_str());
		fl_begin_offscreen(flo);
		fl_draw_box(FL_FLAT_BOX, x, y, 20, 20, fl_rgb_color(0, 255, 0));
		fl_end_offscreen();
	}
	else
	if(RE2::FullMatch(json, "write (\\d+) (\\d+) (.*)", &m0, &m1, &m2)) {
		int x = atoi(m0.c_str());
		int y = atoi(m1.c_str());
		string msg = m2;
		fl_begin_offscreen(flo);
		fl_draw(msg.c_str(), x, y);
		fl_end_offscreen();
	}
	else
	if(RE2::FullMatch(json, "font (\\d+) (\\d+)", &m0, &m1)) {
		/* Fl_Font from Enumerations.H */
		int face = atoi(m0.c_str());
		int size = atoi(m1.c_str());
		fl_font(face, size);
	}
	else
	if(RE2::FullMatch(json, "color ([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})", &m0, &m1, &m2)) {
		int r = strtol(m0.c_str(), NULL, 16);
		int g = strtol(m1.c_str(), NULL, 16);
		int b = strtol(m2.c_str(), NULL, 16);
		fl_color(fl_rgb_color(r,g,b));
	}
	else
	if(RE2::FullMatch(json, "chess (\\d+) (\\d+) (.*)", &m0, &m1, &m2)) {
		int x = atoi(m0.c_str());
		int y = atoi(m1.c_str());
		string fen = m2;

		ChessBoard cb(0,0,1,1);
		cb.fenSet(fen);
		/* let chessboard draw into offscreen buf */
		Fl_Offscreen cbo = fl_create_offscreen(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
		fl_begin_offscreen(cbo);
		cb.draw();
		fl_end_offscreen();
		/* copy chess offscreen buf into our main offscreen buf */
		fl_begin_offscreen(flo);
		fl_copy_offscreen(x, y, CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT, cbo, 0, 0);
		fl_end_offscreen();
		/* delete the chess buffer */
		fl_delete_offscreen(cbo);
	}

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


