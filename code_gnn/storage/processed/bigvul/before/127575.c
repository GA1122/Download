void FreePixmap(Display* display, XID pixmap) {
  XFreePixmap(display, pixmap);
}
