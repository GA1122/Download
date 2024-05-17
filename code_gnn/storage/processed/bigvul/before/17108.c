void OxideQQuickWebView::goForward() {
  Q_D(OxideQQuickWebView);

  if (!d->proxy_) {
    return;
  }

  d->proxy_->goForward();
}
