void OxideQQuickWebViewPrivate::locationBarShow(bool animate) {
  if (!proxy_) {
    return;
  }

  proxy_->locationBarShow(animate);
}
