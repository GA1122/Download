int OxideQQuickWebViewPrivate::locationBarHeight() {
  if (!proxy_) {
    return construct_props_->location_bar_height;
  }

  return proxy_->locationBarHeight();
}
