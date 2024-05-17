void OxideQQuickWebView::touchEvent(QTouchEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::touchEvent(event);
  d->contents_view_->handleTouchEvent(event);
}
