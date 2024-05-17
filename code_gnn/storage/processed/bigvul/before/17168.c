void OxideQQuickWebViewPrivate::setLocationBarMode(
    oxide::qt::LocationBarMode mode) {
  if (!proxy_) {
    construct_props_->location_bar_mode = mode;
  } else {
    proxy_->setLocationBarMode(mode);
  }
}
