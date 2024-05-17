void Compositor::SetBackgroundColor(SkColor color) {
  host_->set_background_color(color);
  SetDisplayColorSpace(output_color_space_, sdr_white_level_);
  ScheduleDraw();
}
