  virtual void BeginPrintMode(float width, float height) {
    DCHECK(!printed_page_width_);
    printed_page_width_ = width;
    printed_page_height_ = height;
    PrintContext::BeginPrintMode(printed_page_width_, height);
  }
