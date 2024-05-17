bool OxideQQuickWebView::canGoForward() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return false;
  }

  return d->proxy_->canGoForward();
}
