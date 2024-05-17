void OxideQQuickWebViewPrivate::FrameRemoved(QObject* frame) {
  Q_Q(OxideQQuickWebView);

  emit q->frameRemoved(qobject_cast<OxideQQuickWebFrame*>(frame));
}
