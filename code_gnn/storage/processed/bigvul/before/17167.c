void OxideQQuickWebViewPrivate::setLocationBarHeight(int height) {
  if (!proxy_) {
    construct_props_->location_bar_height = height;
  } else {
    proxy_->setLocationBarHeight(height);
  }
}
