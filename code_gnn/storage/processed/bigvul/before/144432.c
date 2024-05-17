ExtensionInstallDialogView::~ExtensionInstallDialogView() {
  if (!handled_result_)
    delegate_->InstallUIAbort(true);
}
