bool ExtensionInstallPrompt::Prompt::ShouldDisplayRevokeFilesButton() const {
  return !retained_files_.empty();
}
