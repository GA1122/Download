void OxideQQuickWebView::keyReleaseEvent(QKeyEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::keyReleaseEvent(event);
  d->contents_view_->handleKeyReleaseEvent(event);
}
