void Instance::CalculateBackgroundParts() {
  background_parts_.clear();
  int v_scrollbar_thickness =
      GetScaled(v_scrollbar_.get() ? GetScrollbarReservedThickness() : 0);
  int h_scrollbar_thickness =
      GetScaled(h_scrollbar_.get() ? GetScrollbarReservedThickness() : 0);
  int width_without_scrollbar = std::max(
      plugin_size_.width() - v_scrollbar_thickness, 0);
  int height_without_scrollbar = std::max(
      plugin_size_.height() - h_scrollbar_thickness, 0);
  int left_width = available_area_.x();
  int right_start = available_area_.right();
  int right_width = abs(width_without_scrollbar - available_area_.right());
  int bottom = std::min(available_area_.bottom(), height_without_scrollbar);

  BackgroundPart part = {
    pp::Rect(0, 0, left_width, bottom),
    kBackgroundColor
  };
  if (!part.location.IsEmpty())
    background_parts_.push_back(part);
  part.location = pp::Rect(right_start, 0, right_width, bottom);
  if (!part.location.IsEmpty())
    background_parts_.push_back(part);
  part.location = pp::Rect(
      0, bottom, width_without_scrollbar, height_without_scrollbar - bottom);
  if (!part.location.IsEmpty())
    background_parts_.push_back(part);

  if (h_scrollbar_thickness
#if defined(OS_MACOSX)
      ||
#else
      &&
#endif
      v_scrollbar_thickness) {
    part.color = 0xFFFFFFFF;
    part.location = pp::Rect(plugin_size_.width() - v_scrollbar_thickness,
                             plugin_size_.height() - h_scrollbar_thickness,
                             h_scrollbar_thickness,
                             v_scrollbar_thickness);
    background_parts_.push_back(part);
  }
}
