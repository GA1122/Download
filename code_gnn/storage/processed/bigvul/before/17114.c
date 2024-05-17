bool OxideQQuickWebView::incognito() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return d->construct_props_->incognito;
  }

  return d->proxy_->incognito();
}
