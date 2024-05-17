void OxideQQuickWebView::setFullscreen(bool fullscreen) {
  Q_D(OxideQQuickWebView);

  if (fullscreen == this->fullscreen()) {
    return;
  }

  if (!d->proxy_) {
    d->construct_props_->fullscreen = fullscreen;
  } else {
    d->proxy_->setFullscreen(fullscreen);
  }

  emit fullscreenChanged();
}
