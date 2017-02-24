// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "Driver.h"
#include "logic.h"

ACanvas* Driver::make_window() {
  { canvas = new ACanvas(0, 0, 1357, 660, "canvas");
    canvas->box(FL_FLAT_BOX);
    canvas->color(FL_BACKGROUND_COLOR);
    canvas->selection_color(FL_BACKGROUND_COLOR);
    canvas->labeltype(FL_NO_LABEL);
    canvas->labelfont(0);
    canvas->labelsize(14);
    canvas->labelcolor(FL_FOREGROUND_COLOR);
    canvas->user_data((void*)(this));
    canvas->align(Fl_Align(FL_ALIGN_TOP));
    canvas->when(FL_WHEN_RELEASE);
    canvas->end();
  } // ACanvas* canvas
  return canvas;
}

int main(int argc, char **argv) {
  Driver drv;
  ACanvas *w = drv.make_window();
  
  onGuiInit(&drv, argc, argv);
  
  //Fl::add_idle(onIdle, &gui);
  w->end();
  
  /* enable multithreading */
  Fl::lock();
  
  w->show();
  /* all-in-one is Fl::run() but we also have Fl::wait() and Fl::check() and Fl::ready() */
  int rc = Fl::run();
  onExit();
  return rc;
  /* fluid will insert Fl::run() here... */
  return Fl::run();
}
