qreal OxideQQuickWebView::viewportHeight() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return 0.f;
  }

  return const_cast<OxideQQuickWebViewPrivate*>(
      d)->proxy_->compositorFrameViewportSize().height();
}
