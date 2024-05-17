OxideQQuickWebFrame* OxideQQuickWebView::rootFrame() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return nullptr;
  }

  QObject* frame = d->proxy_->rootFrame();
  if (!frame) {
    return nullptr;
  }

  return qobject_cast<OxideQQuickWebFrame*>(frame);
}
