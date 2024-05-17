base::string16 ExtensionInstallPrompt::Prompt::GetPermissionsHeading(
    PermissionsType permissions_type) const {
  switch (permissions_type) {
    case REGULAR_PERMISSIONS:
      return l10n_util::GetStringUTF16(kPermissionsHeaderIds[type_]);
    case WITHHELD_PERMISSIONS:
      return l10n_util::GetStringUTF16(IDS_EXTENSION_PROMPT_WITHHELD);
    case ALL_PERMISSIONS:
    default:
      NOTREACHED();
      return base::string16();
  }
}
