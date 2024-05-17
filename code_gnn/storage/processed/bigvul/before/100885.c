string16 ExtensionInstallUI::Prompt::GetAcceptButtonLabel() const {
  return l10n_util::GetStringUTF16(kAcceptButtonIds[type_]);
}
