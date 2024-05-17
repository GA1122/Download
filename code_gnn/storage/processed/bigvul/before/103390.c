  void Update() {
    if (needs_update_) {
      needs_update_ = false;
      CGDirectDisplayID mainDevice = CGMainDisplayID();
      int width = CGDisplayPixelsWide(mainDevice);
      int height = CGDisplayPixelsHigh(mainDevice);
      if (width != size_.width() || height != size_.height()) {
        size_.SetSize(width, height);
        bytes_per_row_ = width * sizeof(uint32_t);
        size_t buffer_size = width * height * sizeof(uint32_t);
        ptr_.reset(new uint8[buffer_size]);
      }
    }
  }
