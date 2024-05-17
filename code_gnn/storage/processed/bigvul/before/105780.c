int Label::GetHeightForWidth(int w) {
  if (!is_multi_line_)
    return View::GetHeightForWidth(w);

  w = std::max(0, w - GetInsets().width());
  int h = font_.GetHeight();
  gfx::CanvasSkia::SizeStringInt(text_, font_, &w, &h,
                                 ComputeMultiLineFlags());
  return h + GetInsets().height();
}
