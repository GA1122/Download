void OxideQQuickWebView::setPromptDialog(QQmlComponent* promptDialog) {
  Q_D(OxideQQuickWebView);

  if (d->prompt_dialog_ == promptDialog) {
    return;
  }

  d->prompt_dialog_ = promptDialog;
  emit promptDialogChanged();
}
