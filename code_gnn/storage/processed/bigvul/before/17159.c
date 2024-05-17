void OxideQQuickWebView::setCanTemporarilyRunInsecureContent(bool allow) {
  Q_D(OxideQQuickWebView);

  if (!d->proxy_) {
    return;
  }

  d->proxy_->setCanTemporarilyRunInsecureContent(allow);
}
