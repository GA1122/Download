base::string16 ExtensionInstallPrompt::Prompt::GetDialogTitle() const {
  int id = kTitleIds[type_];
  if (type_ == BUNDLE_INSTALL_PROMPT ||
      type_ == DELEGATED_BUNDLE_PERMISSIONS_PROMPT) {
    return bundle_->GetHeadingTextFor(BundleInstaller::Item::STATE_PENDING);
  }
  if (type_ == DELEGATED_PERMISSIONS_PROMPT) {
    return l10n_util::GetStringFUTF16(id, base::UTF8ToUTF16(extension_->name()),
                                      base::UTF8ToUTF16(delegated_username_));
  }
  if (type_ == EXTERNAL_INSTALL_PROMPT) {
    if (extension_->is_app())
      id = IDS_EXTENSION_EXTERNAL_INSTALL_PROMPT_TITLE_APP;
    else if (extension_->is_theme())
      id = IDS_EXTENSION_EXTERNAL_INSTALL_PROMPT_TITLE_THEME;
    else
      id = IDS_EXTENSION_EXTERNAL_INSTALL_PROMPT_TITLE_EXTENSION;
  }
  return l10n_util::GetStringFUTF16(id, base::UTF8ToUTF16(extension_->name()));
}
