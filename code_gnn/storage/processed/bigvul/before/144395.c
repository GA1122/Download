bool ExtensionInstallPrompt::Prompt::ShouldDisplayRevokeButton() const {
  return !retained_files_.empty() || !retained_device_messages_.empty();
}
