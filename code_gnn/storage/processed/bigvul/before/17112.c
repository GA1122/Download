QUrl OxideQQuickWebView::hoveredUrl() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return QUrl();
  }

  return d->proxy_->targetUrl();
}
