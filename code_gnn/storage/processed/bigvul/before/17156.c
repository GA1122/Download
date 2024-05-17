void OxideQQuickWebView::setAlertDialog(QQmlComponent* alertDialog) {
  Q_D(OxideQQuickWebView);

  if (d->alert_dialog_ == alertDialog) {
    return;
  }

  d->alert_dialog_ = alertDialog;
  emit alertDialogChanged();
}
