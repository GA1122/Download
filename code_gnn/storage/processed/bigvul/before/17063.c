void OxideQQuickWebViewPrivate::ZoomLevelChanged() {
  Q_Q(OxideQQuickWebView);

  emit q->zoomFactorChanged();
}
