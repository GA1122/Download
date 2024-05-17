::Cursor CreateInvisibleCursor() {
  Display* xdisplay = ui::GetXDisplay();
  ::Cursor invisible_cursor;
  char nodata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  XColor black;
  black.red = black.green = black.blue = 0;
  Pixmap blank = XCreateBitmapFromData(xdisplay,
                                       DefaultRootWindow(xdisplay),
                                       nodata, 8, 8);
  invisible_cursor = XCreatePixmapCursor(xdisplay, blank, blank,
                                         &black, &black, 0, 0);
  XFreePixmap(xdisplay, blank);
  return invisible_cursor;
}
