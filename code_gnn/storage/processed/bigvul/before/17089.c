void OxideQQuickWebView::dragLeaveEvent(QDragLeaveEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::dragLeaveEvent(event);
  d->contents_view_->handleDragLeaveEvent(event);
}
