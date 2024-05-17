void OxideQQuickWebView::mouseReleaseEvent(QMouseEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::mouseReleaseEvent(event);
  d->contents_view_->handleMouseReleaseEvent(event);
}
