void OxideQQuickWebView::keyPressEvent(QKeyEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::keyPressEvent(event);
  d->contents_view_->handleKeyPressEvent(event);
}
