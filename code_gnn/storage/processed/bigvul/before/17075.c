void OxideQQuickWebView::connectNotify(const QMetaMethod& signal) {
  Q_D(OxideQQuickWebView);

  Q_ASSERT(thread() == QThread::currentThread());

#define VIEW_SIGNAL(sig) QMetaMethod::fromSignal(&OxideQQuickWebView::sig)
  if (signal == VIEW_SIGNAL(newViewRequested) && d->proxy_) {
    d->proxy_->updateWebPreferences();
  } else if (signal == VIEW_SIGNAL(loadingChanged)) {
    WARN_DEPRECATED_API_USAGE() <<
        "OxideQQuickWebView: loadingChanged is deprecated. Please connect "
        "loadEvent if you want load event notifications, or "
        "loadingStateChanged if you want to detect changes to the loading "
        "property";
    d->using_old_load_event_signal_ = true;
  }
#undef VIEW_SIGNAL
}
