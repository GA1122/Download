void OxideQQuickWebView::wheelEvent(QWheelEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::wheelEvent(event);
  d->contents_view_->handleWheelEvent(event);
}
