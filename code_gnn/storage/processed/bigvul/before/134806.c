gfx::Size TouchEventConverterEvdev::GetTouchscreenSize() const {
  return gfx::Size(x_num_tuxels_, y_num_tuxels_);
}
