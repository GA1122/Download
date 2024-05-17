void OxideQQuickWebView::stop() {
  Q_D(OxideQQuickWebView);

  if (!d->proxy_) {
    return;
  }

  d->proxy_->stop();
}
