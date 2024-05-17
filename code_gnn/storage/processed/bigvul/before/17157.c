void OxideQQuickWebView::setBeforeUnloadDialog(
    QQmlComponent* beforeUnloadDialog) {
  Q_D(OxideQQuickWebView);

  if (d->before_unload_dialog_ == beforeUnloadDialog) {
    return;
  }

  d->before_unload_dialog_ = beforeUnloadDialog;
  emit beforeUnloadDialogChanged();
}
