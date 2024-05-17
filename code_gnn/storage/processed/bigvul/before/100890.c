string16 ExtensionInstallUI::Prompt::GetPermissionsHeader() const {
  return l10n_util::GetStringUTF16(kPermissionsHeaderIds[type_]);
}
