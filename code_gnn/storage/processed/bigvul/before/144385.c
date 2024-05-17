void ExtensionInstallPrompt::OnInstallFailure(
    const extensions::CrxInstallError& error) {
  install_ui_->OnInstallFailure(error);
}
