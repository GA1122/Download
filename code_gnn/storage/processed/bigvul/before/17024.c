bool OxideQQuickWebViewPrivate::CanCreateWindows() const {
  Q_Q(const OxideQQuickWebView);

  return q->receivers(SIGNAL(newViewRequested(OxideQNewViewRequest*))) > 0;
}
