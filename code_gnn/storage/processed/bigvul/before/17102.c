int OxideQQuickWebViewPrivate::getNavigationEntryCount() const {
  if (!proxy_) {
    return 0;
  }

  return proxy_->getNavigationEntryCount();
}
