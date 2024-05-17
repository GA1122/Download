OxideQQuickWebView::~OxideQQuickWebView() {
  Q_D(OxideQQuickWebView);

  if (d->contextHandle()) {
    d->detachContextSignals(
        OxideQQuickWebContextPrivate::get(
          qobject_cast<OxideQQuickWebContext*>(d->contextHandle())));
  }

  while (d->messageHandlers().size() > 0) {
    delete d->messageHandlers().at(0);
  }

  d->proxy_->teardownFrameTree();
}
