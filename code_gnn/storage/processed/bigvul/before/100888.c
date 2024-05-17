string16 ExtensionInstallUI::Prompt::GetPermission(int index) const {
  return l10n_util::GetStringFUTF16(
      IDS_EXTENSION_PERMISSION_LINE, permissions_[index]);
}
