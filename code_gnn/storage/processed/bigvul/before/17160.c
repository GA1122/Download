void OxideQQuickWebView::setConfirmDialog(QQmlComponent* confirmDialog) {
  Q_D(OxideQQuickWebView);

  if (d->confirm_dialog_ == confirmDialog) {
    return;
  }

  d->confirm_dialog_ = confirmDialog;
  emit confirmDialogChanged();
}
