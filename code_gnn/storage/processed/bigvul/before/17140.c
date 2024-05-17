void OxideQQuickWebView::mouseMoveEvent(QMouseEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::mouseMoveEvent(event);
  d->contents_view_->handleMouseMoveEvent(event);
}
