void OxideQQuickWebViewPrivate::FaviconChanged() {
  Q_Q(OxideQQuickWebView);

  emit q->iconChanged();
}
