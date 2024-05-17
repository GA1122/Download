bool OxideQQuickWebView::fullscreen() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return d->construct_props_->fullscreen;
  }

  return d->proxy_->fullscreen();
}
