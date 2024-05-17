int OxideQQuickWebViewPrivate::messageHandler_count(
    QQmlListProperty<OxideQQuickScriptMessageHandler>* prop) {
  OxideQQuickWebViewPrivate* p = OxideQQuickWebViewPrivate::get(
      static_cast<OxideQQuickWebView*>(prop->object));

  return p->messageHandlers().size();
}
