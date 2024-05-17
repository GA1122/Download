bool OxideQQuickWebViewPrivate::locationBarAnimated() const {
  if (!proxy_) {
    return construct_props_->location_bar_animated;
  }

  return proxy_->locationBarAnimated();
}
