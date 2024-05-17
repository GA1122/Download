base::string16 ExtensionInstallPrompt::Prompt::GetUserCount() const {
  CHECK(AllowWebstoreData(type_));

  if (show_user_count_) {
    return l10n_util::GetStringFUTF16(IDS_EXTENSION_USER_COUNT,
                                      base::UTF8ToUTF16(localized_user_count_));
  }
  return base::string16();
}
