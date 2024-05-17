    ~XCustomCursor() {
      XcursorImageDestroy(image_);
      XFreeCursor(GetXDisplay(), cursor_);
    }
