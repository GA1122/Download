void OutOfProcessInstance::UpdateTickMarks(
    const std::vector<pp::Rect>& tickmarks) {
  float inverse_scale = 1.0f / device_scale_;
  std::vector<pp::Rect> scaled_tickmarks = tickmarks;
  for (size_t i = 0; i < scaled_tickmarks.size(); i++)
    ScaleRect(inverse_scale, &scaled_tickmarks[i]);
  tickmarks_ = scaled_tickmarks;
}
