void OxideQQuickWebView::setFilePicker(QQmlComponent* filePicker) {
  Q_D(OxideQQuickWebView);

  if (d->file_picker_ == filePicker) {
    return;
  }

  d->file_picker_ = filePicker;
  emit filePickerChanged();
}
