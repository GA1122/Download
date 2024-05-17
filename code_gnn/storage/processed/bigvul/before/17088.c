void OxideQQuickWebView::dragEnterEvent(QDragEnterEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::dragEnterEvent(event);
  d->contents_view_->handleDragEnterEvent(event);
}
