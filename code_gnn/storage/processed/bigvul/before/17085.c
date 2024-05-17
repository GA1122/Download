QString OxideQQuickWebView::currentState() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return QString();
  }

  return QString::fromLocal8Bit(d->proxy_->currentState().toBase64());
}
