EGLNativeWindowType RenderingHelper::PlatformCreateWindow(int top_left_x,
                                                          int top_left_y) {
  int depth = DefaultDepth(x_display_, DefaultScreen(x_display_));

  XSetWindowAttributes window_attributes;
  window_attributes.background_pixel =
      BlackPixel(x_display_, DefaultScreen(x_display_));
  window_attributes.override_redirect = true;

  Window x_window = XCreateWindow(
      x_display_, DefaultRootWindow(x_display_),
      top_left_x, top_left_y, width_, height_,
      0  ,
      depth, CopyFromParent  , CopyFromParent  ,
      (CWBackPixel | CWOverrideRedirect), &window_attributes);
  x_windows_.push_back(x_window);
  XStoreName(x_display_, x_window, "VideoDecodeAcceleratorTest");
  XSelectInput(x_display_, x_window, ExposureMask);
  XMapWindow(x_display_, x_window);
  return x_window;
}
