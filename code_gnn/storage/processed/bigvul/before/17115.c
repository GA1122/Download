void OxideQQuickWebView::inputMethodEvent(QInputMethodEvent* event) {
  Q_D(OxideQQuickWebView);

  QQuickItem::inputMethodEvent(event);
  d->contents_view_->handleInputMethodEvent(event);
}
