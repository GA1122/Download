    XCustomCursor(XcursorImage* image)
        : image_(image),
          ref_(1) {
      cursor_ = XcursorImageLoadCursor(GetXDisplay(), image);
    }
