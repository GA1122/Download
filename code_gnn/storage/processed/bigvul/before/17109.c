void OxideQQuickWebView::hoverEnterEvent(QHoverEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::hoverEnterEvent(event);
  d->contents_view_->handleHoverEnterEvent(event);
}
