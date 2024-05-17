string16 ExtensionInstallUI::Prompt::GetAbortButtonLabel() const {
  CHECK(HasAbortButtonLabel());
  return l10n_util::GetStringUTF16(kAbortButtonIds[type_]);
}
