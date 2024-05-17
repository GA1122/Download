static int rpng_x_create_window(void)
{
    uch *xdata;
 int need_colormap = FALSE;
 int screen, pad;
    ulg bg_pixel = 0L;
    ulg attrmask;
 Window root;
 XEvent e;
 XGCValues gcvalues;
 XSetWindowAttributes attr;
 XTextProperty windowName, *pWindowName = &windowName;
 XTextProperty iconName, *pIconName = &iconName;
 XVisualInfo visual_info;
 XSizeHints *size_hints;
 XWMHints *wm_hints;
 XClassHint *class_hints;


    screen = DefaultScreen(display);
    depth = DisplayPlanes(display, screen);
    root = RootWindow(display, screen);

#ifdef DEBUG
 XSynchronize(display, True);
#endif

#if 0
 
 if (  depth != 16 && depth != 24 && depth != 32) {
        fprintf(stderr,
 "screen depth %d not supported (only 16-, 24- or 32-bit TrueColor)\n",
          depth);
 return 2;
 }

 XMatchVisualInfo(display, screen, depth,
 (depth == 8)? PseudoColor : TrueColor, &visual_info);
    visual = visual_info.visual;
#else
 if (depth != 16 && depth != 24 && depth != 32) {
 int visuals_matched = 0;

 Trace((stderr, "default depth is %d:  checking other visuals\n",
          depth))

  
        visual_info.screen = screen;
        visual_info.depth = 24;
        visual_list = XGetVisualInfo(display,
 VisualScreenMask | VisualDepthMask, &visual_info, &visuals_matched);
 if (visuals_matched == 0) {
 
            fprintf(stderr, "default screen depth %d not supported, and no"
 " 24-bit visuals found\n", depth);
 return 2;
 }
 Trace((stderr, "XGetVisualInfo() returned %d 24-bit visuals\n",
          visuals_matched))
        visual = visual_list[0].visual;
        depth = visual_list[0].depth;
 
        have_nondefault_visual = TRUE;
        need_colormap = TRUE;
 } else {
 XMatchVisualInfo(display, screen, depth, TrueColor, &visual_info);
        visual = visual_info.visual;
 }
#endif

 RMask = visual->red_mask;
 GMask = visual->green_mask;
 BMask = visual->blue_mask;

 
 if (depth == 8 || need_colormap) {
        colormap = XCreateColormap(display, root, visual, AllocNone);
 if (!colormap) {
            fprintf(stderr, "XCreateColormap() failed\n");
 return 2;
 }
        have_colormap = TRUE;
 }
 if (depth == 15 || depth == 16) {
 RShift = 15 - rpng_x_msb(RMask);  
 GShift = 15 - rpng_x_msb(GMask);
 BShift = 15 - rpng_x_msb(BMask);
 } else if (depth > 16) {
#define NO_24BIT_MASKS
#ifdef NO_24BIT_MASKS
 RShift = rpng_x_msb(RMask) - 7;  
 GShift = rpng_x_msb(GMask) - 7;
 BShift = rpng_x_msb(BMask) - 7;
#else
 RShift = 7 - rpng_x_msb(RMask);  
 GShift = 7 - rpng_x_msb(GMask);
 BShift = 7 - rpng_x_msb(BMask);
#endif
 }
 if (depth >= 15 && (RShift < 0 || GShift < 0 || BShift < 0)) {
        fprintf(stderr, "rpng internal logic error:  negative X shift(s)!\n");
 return 2;
 }

 

    attr.backing_store = Always;
    attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask;
    attrmask = CWBackingStore | CWEventMask;
 if (have_nondefault_visual) {
        attr.colormap = colormap;
        attr.background_pixel = 0;
        attr.border_pixel = 1;
        attrmask |= CWColormap | CWBackPixel | CWBorderPixel;
 }

    window = XCreateWindow(display, root, 0, 0, image_width, image_height, 0,
      depth, InputOutput, visual, attrmask, &attr);

 if (window == None) {
        fprintf(stderr, "XCreateWindow() failed\n");
 return 2;
 } else
        have_window = TRUE;

 if (depth == 8)
 XSetWindowColormap(display, window, colormap);

 if (!XStringListToTextProperty(&window_name, 1, pWindowName))
        pWindowName = NULL;
 if (!XStringListToTextProperty(&icon_name, 1, pIconName))
        pIconName = NULL;

  

 if ((size_hints = XAllocSizeHints()) != NULL) {
  
        size_hints->flags = PMinSize | PMaxSize;
        size_hints->min_width = size_hints->max_width = (int)image_width;
        size_hints->min_height = size_hints->max_height = (int)image_height;
 }

 if ((wm_hints = XAllocWMHints()) != NULL) {
        wm_hints->initial_state = NormalState;
        wm_hints->input = True;
  
        wm_hints->flags = StateHint | InputHint   ;
 }

 if ((class_hints = XAllocClassHint()) != NULL) {
        class_hints->res_name = res_name;
        class_hints->res_class = res_class;
 }

 XSetWMProperties(display, window, pWindowName, pIconName, NULL, 0,
      size_hints, wm_hints, class_hints);

  
 if (pWindowName)
 XFree(pWindowName->value);
 if (pIconName)
 XFree(pIconName->value);
 if (size_hints)
 XFree(size_hints);
 if (wm_hints)
 XFree(wm_hints);
 if (class_hints)
 XFree(class_hints);

 XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, &gcvalues);
    have_gc = TRUE;

 

 if (depth == 24 || depth == 32) {
        bg_pixel = ((ulg)bg_red   << RShift) |
 ((ulg)bg_green << GShift) |
 ((ulg)bg_blue  << BShift);
 } else if (depth == 16) {
        bg_pixel = ((((ulg)bg_red   << 8) >> RShift) & RMask) |
 ((((ulg)bg_green << 8) >> GShift) & GMask) |
 ((((ulg)bg_blue  << 8) >> BShift) & BMask);
 } else   {

  

 }

 XSetForeground(display, gc, bg_pixel);
 XFillRectangle(display, window, gc, 0, 0, image_width, image_height);

 

 do
 XNextEvent(display, &e);
 while (e.type != Expose || e.xexpose.count);

 XFlush(display);

 

 if (depth == 24 || depth == 32) {
        xdata = (uch *)malloc(4*image_width*image_height);
        pad = 32;
 } else if (depth == 16) {
        xdata = (uch *)malloc(2*image_width*image_height);
        pad = 16;
 } else   {
        xdata = (uch *)malloc(image_width*image_height);
        pad = 8;
 }

 if (!xdata) {
        fprintf(stderr, PROGNAME ":  unable to allocate image memory\n");
 return 4;
 }

    ximage = XCreateImage(display, visual, depth, ZPixmap, 0,
 (char *)xdata, image_width, image_height, pad, 0);

 if (!ximage) {
        fprintf(stderr, PROGNAME ":  XCreateImage() failed\n");
        free(xdata);
 return 3;
 }

  

    ximage->byte_order = MSBFirst;

 return 0;

}  
