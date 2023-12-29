// Cairo Basic X11 Tutorial
//
// Author: Jacob Christensen
//
// References: 
//  https://tronche.com/gui/x/xlib/
//  https://www.cairographics.org/FAQ/
//  https://www.cairographics.org/tutorial/
//  https://gitlab.com/cairo/cairo-demos/-/blob/master/X11/cairo-demo.c?ref_type=heads

#include <X11/Xlib.h>
#include <cairo.h>
#include <cairo-xlib.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000

void drawCircle(cairo_t *, double, double, double, double);

void drawCircle(cairo_t *cxt, double radius, double r, double g, double b) {

    cairo_set_source_rgb(cxt, r, g, b);
    cairo_arc(cxt, 500.0, 500.0, radius, 0.0, 2 * M_PI);
    cairo_fill(cxt);

}

int main(void) {

    Display *dsp;                   // X Server Connection
    int screen;                     // Screen number
    Drawable da;                    // X11 Drawable (Window)
    cairo_surface_t *surface;       // Surface
    cairo_t         *context;       // Context

    // Open connection to X Server
    dsp = XOpenDisplay(NULL);
    if (dsp == NULL) {
        printf("Error: Can't open connection to X Server");
        exit(1);
    }

    // Get screen number
    screen = DefaultScreen(dsp);

    // Create Window
    da = XCreateSimpleWindow(
            dsp,
            DefaultRootWindow(dsp),
            0,
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0,
            0,
            0x00ffffff
    );

    // Specify what events we want to receive from X Server
    XSelectInput(dsp, da, ExposureMask | KeyPressMask);

    // Map Window so it's viewable on the screen (...I Think)
    XMapWindow(dsp, da);

    // Create cairo surface
    surface = cairo_xlib_surface_create(
            dsp,
            da,
            DefaultVisual(dsp, screen),
            WINDOW_HEIGHT,
            WINDOW_WIDTH
    );
    cairo_xlib_surface_set_size(surface, WINDOW_HEIGHT, WINDOW_WIDTH);
    
    // Create Context
    context = cairo_create(surface);
    
    XEvent xev;
    double r = 1.0;
    double g = 0.0;
    double b = 0.0;

    while(1) {

        XNextEvent(dsp, &xev);
        switch (xev.type) {
            case KeyPress:
                // Toggle color of circle, or quit if q is pressed
                // TODO: Find out why keycode and XStringToKeysym("Q")
                //  doesn't work
                if (xev.xkey.keycode == 24) {
                    cairo_destroy(context);
                    cairo_surface_destroy(surface);
                    XDestroyWindow(dsp, screen);
                    exit(0);
                }
                else {
                    // Toggle color and re-draw
                    r = r ? 0.0 : 1.0;
                    b = b ? 0.0 : 1.0;
                    drawCircle(context, 200, r, g, b);
                }
                break;

            case Expose:
                // Draw initial circle when window is started by X
                drawCircle(context, 200, r, g, b);
                break;
        }

    }

    return EXIT_SUCCESS;

}
