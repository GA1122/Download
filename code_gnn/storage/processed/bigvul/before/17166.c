void OxideQQuickWebViewPrivate::setLocationBarAnimated(bool animated) {
  if (!proxy_) {
    construct_props_->location_bar_animated = animated;
  } else {
    proxy_->setLocationBarAnimated(animated);
  }
}
