int OxideQQuickWebViewPrivate::locationBarContentOffset() {
  if (!proxy_) {
    return 0;
  }

  return proxy_->locationBarContentOffset();
}
