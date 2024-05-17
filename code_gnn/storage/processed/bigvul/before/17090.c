void OxideQQuickWebView::dragMoveEvent(QDragMoveEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::dragMoveEvent(event);
  d->contents_view_->handleDragMoveEvent(event);
}
