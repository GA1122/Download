void OxideQQuickWebViewPrivate::contextDestroyed() {
  Q_Q(OxideQQuickWebView);

  if (construct_props_) {
    construct_props_->context = nullptr;
  }

  emit q->contextChanged();
}
