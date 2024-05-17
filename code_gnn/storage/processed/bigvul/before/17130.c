int OxideQQuickWebViewPrivate::locationBarOffset() {
  if (!proxy_) {
    return 0;
  }

  return proxy_->locationBarOffset();
}
