bool OxideQQuickWebView::canGoBack() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return false;
  }

  return d->proxy_->canGoBack();
}
