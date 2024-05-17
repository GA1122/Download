void ExtensionUninstaller::OnExtensionUninstallDialogClosed(
    bool did_start_uninstall,
    const base::string16& error) {
  controller_->OnCloseChildDialog();
  CleanUp();
}
