void OxideQQuickWebView::focusOutEvent(QFocusEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::focusOutEvent(event);
  d->contents_view_->handleFocusOutEvent(event);
}
