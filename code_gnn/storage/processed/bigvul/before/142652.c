void WebstoreStandaloneInstaller::ProceedWithInstallPrompt() {
  install_prompt_ = CreateInstallPrompt();
  if (install_prompt_.get()) {
    ShowInstallUI();
  } else {
    OnInstallPromptDone(ExtensionInstallPrompt::Result::ACCEPTED);
  }
}
