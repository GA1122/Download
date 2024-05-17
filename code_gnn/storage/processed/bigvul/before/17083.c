QObject* OxideQQuickWebViewPrivate::contextHandle() const {
  if (!proxy_) {
    return construct_props_->context;
  }

  return proxy_->context();
}
