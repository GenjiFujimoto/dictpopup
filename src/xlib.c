#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>

#include <glib.h>

#include "util.h"

/*
 * Waits for the next clipboard event. 
 */
i32
clipnotify(void)
{
	Display *disp;
	Window root;
	Atom clip;
	XEvent evt;

	disp = XOpenDisplay(NULL);
	if (!disp)
		return 1;

	root = DefaultRootWindow(disp);

	clip = XInternAtom(disp, "CLIPBOARD", False);

	XFixesSelectSelectionInput(disp, root, clip,
				   XFixesSetSelectionOwnerNotifyMask);

	XNextEvent(disp, &evt);

	XCloseDisplay(disp);

	return 0;
}

/*
 * Returns the window title of the currently active window 
 */
char*
getwindowname(void)
{
	Display *dpy;
	Window focused;
	XTextProperty text_prop;
	int revert_to;

	if (!(dpy = XOpenDisplay(NULL)))
	{
		fprintf(stderr, "ERROR: Can't open X display for retrieving the window title. Are you using Linux with X11?");
		return NULL;
	}

	XGetInputFocus(dpy, &focused, &revert_to);

	if (!XGetWMName(dpy, focused, &text_prop))
	{
		fprintf(stderr, "ERROR: Could not obtain window name. \n");
		return NULL;
	}
	XCloseDisplay(dpy);

	/* char *wname = g_strdup((char *)text_prop.value); */
	/* XFree(text_prop.value); */
	return (char*)text_prop.value;
}
