gfx::Size Label::GetTextSize() const {
  if (!text_size_valid_) {
    int w = is_multi_line_ ?
        GetAvailableRect().width() : std::numeric_limits<int>::max();
    int h = font_.GetHeight();
    int flags = ComputeMultiLineFlags();
    if (!is_multi_line_)
      flags |= gfx::Canvas::NO_ELLIPSIS;
    gfx::CanvasSkia::SizeStringInt(text_, font_, &w, &h, flags);
    text_size_.SetSize(w, h);
    text_size_valid_ = true;
  }

  return text_size_;
}
