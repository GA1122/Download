void OutOfProcessInstance::CalculateBackgroundParts() {
  background_parts_.clear();
  int left_width = available_area_.x();
  int right_start = available_area_.right();
  int right_width = abs(plugin_size_.width() - available_area_.right());
  int bottom = std::min(available_area_.bottom(), plugin_size_.height());

  BackgroundPart part = {
    pp::Rect(0, 0, left_width, bottom),
    background_color_
  };
  if (!part.location.IsEmpty())
    background_parts_.push_back(part);
  part.location = pp::Rect(right_start, 0, right_width, bottom);
  if (!part.location.IsEmpty())
    background_parts_.push_back(part);
  part.location = pp::Rect(
      0, bottom, plugin_size_.width(), plugin_size_.height() - bottom);
  if (!part.location.IsEmpty())
    background_parts_.push_back(part);
}
