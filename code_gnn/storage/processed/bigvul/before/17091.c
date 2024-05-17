void OxideQQuickWebView::dropEvent(QDropEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::dropEvent(event);
  d->contents_view_->handleDropEvent(event);
}
