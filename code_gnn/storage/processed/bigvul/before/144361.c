base::string16 ExtensionInstallPrompt::Prompt::GetAbortButtonLabel() const {
  return l10n_util::GetStringUTF16(kAbortButtonIds[type_]);
}
