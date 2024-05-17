bool AllowWebstoreData(ExtensionInstallPrompt::PromptType type) {
  return type == ExtensionInstallPrompt::INLINE_INSTALL_PROMPT ||
         type == ExtensionInstallPrompt::EXTERNAL_INSTALL_PROMPT ||
         type == ExtensionInstallPrompt::REPAIR_PROMPT;
}
