void ExtensionInstallDialogView::UpdateInstallResultHistogram(bool accepted)
    const {
  if (prompt_->type() == ExtensionInstallPrompt::INSTALL_PROMPT)
    UMA_HISTOGRAM_BOOLEAN("Extensions.InstallPrompt.Accepted", accepted);
}
