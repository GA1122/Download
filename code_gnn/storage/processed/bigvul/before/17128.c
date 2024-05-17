void OxideQQuickWebViewPrivate::locationBarHide(bool animate) {
  if (!proxy_) {
    return;
  }

  proxy_->locationBarHide(animate);
}
