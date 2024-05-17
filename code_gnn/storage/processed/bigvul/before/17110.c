void OxideQQuickWebView::hoverLeaveEvent(QHoverEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::hoverLeaveEvent(event);
  d->contents_view_->handleHoverLeaveEvent(event);
}
