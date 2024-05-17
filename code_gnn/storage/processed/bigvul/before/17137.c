QList<QObject*>& OxideQQuickWebViewPrivate::messageHandlers() {
  if (!proxy_) {
    return construct_props_->message_handlers;
  }

  return proxy_->messageHandlers();
}
