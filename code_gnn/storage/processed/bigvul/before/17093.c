bool OxideQQuickWebView::event(QEvent* event) {
  if (event->type() == GetPrepareToCloseBypassEventType()) {
    emit prepareToCloseResponse(true);
    return true;
  }

  return QQuickItem::event(event);
}
