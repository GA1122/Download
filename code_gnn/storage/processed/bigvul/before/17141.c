void OxideQQuickWebView::mousePressEvent(QMouseEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::mousePressEvent(event);
  d->contents_view_->handleMousePressEvent(event);
}
