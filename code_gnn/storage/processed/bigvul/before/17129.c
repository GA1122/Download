oxide::qt::LocationBarMode OxideQQuickWebViewPrivate::locationBarMode() const {
  if (!proxy_) {
    return construct_props_->location_bar_mode;
  }

  return proxy_->locationBarMode();
}
