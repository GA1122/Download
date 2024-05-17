     Window* get(Display* dpy)
     {
         if (!window) {
            window = XCreateSimpleWindow(dpy, XDefaultRootWindow(dpy), -1, -1, 1, 1, 0, BlackPixel(dpy, 0), WhitePixel(dpy, 0));
            XSetWindowAttributes attributes;
            attributes.override_redirect = true;
            XChangeWindowAttributes(dpy, window, X11OverrideRedirect, &attributes);
            display = dpy;
            XMapWindow(dpy, window);
        }
 
         return &window;
     }
