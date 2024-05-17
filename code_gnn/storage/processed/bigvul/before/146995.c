  virtual float GetPageShrink(int page_number) const {
    IntRect page_rect = page_rects_[page_number];
    return printed_page_width_ / page_rect.Width();
  }
