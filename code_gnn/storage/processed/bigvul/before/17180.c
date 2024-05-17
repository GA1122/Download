QString OxideQQuickWebView::title() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return QString();
  }

  return d->proxy_->title();
}
