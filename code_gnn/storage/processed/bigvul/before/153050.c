pp::Rect PDFiumEngine::GetVisibleRect() const {
  pp::Rect rv;
  rv.set_x(static_cast<int>(position_.x() / current_zoom_));
  rv.set_y(static_cast<int>(position_.y() / current_zoom_));
  rv.set_width(static_cast<int>(ceil(plugin_size_.width() / current_zoom_)));
  rv.set_height(static_cast<int>(ceil(plugin_size_.height() / current_zoom_)));
  return rv;
}
