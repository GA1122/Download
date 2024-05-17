void OxideQQuickWebView::removeMessageHandler(
    OxideQQuickScriptMessageHandler* handler) {
  Q_D(OxideQQuickWebView);

  if (!handler) {
    qWarning() << "OxideQQuickWebView::removeMessageHandler: NULL handler";
    return;
  }

  if (!d->messageHandlers().contains(handler)) {
    return;
  }

  handler->setParent(nullptr);
  d->messageHandlers().removeOne(handler);

  emit messageHandlersChanged();
}
