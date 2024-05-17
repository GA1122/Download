void OxideQQuickWebView::goBack() {
  Q_D(OxideQQuickWebView);

  if (!d->proxy_) {
    return;
  }

  d->proxy_->goBack();
}
