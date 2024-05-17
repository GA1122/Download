void OxideQQuickWebViewPrivate::TargetURLChanged() {
  Q_Q(OxideQQuickWebView);

  emit q->hoveredUrlChanged();
}
