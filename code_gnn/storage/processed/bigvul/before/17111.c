void OxideQQuickWebView::hoverMoveEvent(QHoverEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::hoverMoveEvent(event);
  d->contents_view_->handleHoverMoveEvent(event);
}
