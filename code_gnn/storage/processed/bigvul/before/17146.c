void OxideQQuickWebView::prepareToClose() {
  Q_D(OxideQQuickWebView);

  if (!d->proxy_) {
    QCoreApplication::postEvent(this,
                                new QEvent(GetPrepareToCloseBypassEventType()));
    return;
  }

  d->proxy_->prepareToClose();
}
