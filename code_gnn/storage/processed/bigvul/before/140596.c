pp::FloatPoint OutOfProcessInstance::BoundScrollOffsetToDocument(
    const pp::FloatPoint& scroll_offset) {
  float max_x = document_size_.width() * zoom_ - plugin_dip_size_.width();
  float x = std::max(std::min(scroll_offset.x(), max_x), 0.0f);
  float min_y = -top_toolbar_height_;
  float max_y = document_size_.height() * zoom_ - plugin_dip_size_.height();
  float y = std::max(std::min(scroll_offset.y(), max_y), min_y);
  return pp::FloatPoint(x, y);
}
