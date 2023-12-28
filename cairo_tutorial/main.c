// Cairo Hello World Tutorial
//
// Author: Jacob Christensen
// References: 
//  https://www.cairographics.org/FAQ/
//  https://www.cairographics.org/tutorial/

#include <X11/Xlib.h>
#include <cairo.h>
#include <cairo-xlib.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

Display *dsp;

cairo_surface_t *cairo_create_x11_surface0(int x, int y) {

    Drawable da;
    int screen;
    cairo_surface_t *sfc;

    if ((dsp = XOpenDisplay(NULL)) == NULL) {
        printf("Error, closing");
        exit(1);
    }

    screen = DefaultScreen(dsp);
    da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp),
        0, 0, x, y, 0, 0, 0);
    XSelectInput(dsp, da, ButtonPressMask | KeyPressMask);
    XMapWindow(dsp, da);

    sfc = cairo_xlib_surface_create(dsp, da,
        DefaultVisual(dsp, screen), x, y);
    cairo_xlib_surface_set_size(sfc, x, y);

    return sfc;
}

int main(void) {

    cairo_surface_t *surface;       // Surface
    cairo_t         *cr;            // Context

    surface = cairo_create_x11_surface0(1000, 1000);
    cr = cairo_create(surface);
    
    /* cairo_set_source_rgb(cr, 0.0, 0.0, 1.0); */
    /* cairo_rectangle(cr, 0.0, 0.0, 1000.0, 1000.0); */
    /* cairo_fill(cr); */

    XEvent xev;
    double r = 1.0;
    double g = 0.0;
    double b = 0.0;
    while(1) {

        XNextEvent(dsp, &xev);
        printf("Event Type: %d\n", xev.type);
        cairo_set_source_rgb(cr, r, g, b);
        cairo_arc(cr, 500.0, 500.0, 200, 0.0, 2 * M_PI);
        cairo_fill(cr);

        r = r ? 0.0 : 1.0;
        b = b ? 0.0 : 1.0;
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return EXIT_SUCCESS;
}
