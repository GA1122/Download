int OxideQQuickWebViewPrivate::getNavigationCurrentEntryIndex() const {
  if (!proxy_) {
    return -1;
  }

  return proxy_->getNavigationCurrentEntryIndex();
}
