void OxideQQuickWebView::reload() {
  Q_D(OxideQQuickWebView);

  if (!d->proxy_) {
    return;
  }

  d->proxy_->reload();
}
