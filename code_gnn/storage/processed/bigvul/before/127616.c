void PutARGBImage(Display* display,
                  void* visual, int depth,
                  XID pixmap, void* pixmap_gc,
                  const uint8* data,
                  int width, int height) {
  PutARGBImage(display,
               visual, depth,
               pixmap, pixmap_gc,
               data, width, height,
               0, 0,  
               0, 0,  
               width, height);
}
