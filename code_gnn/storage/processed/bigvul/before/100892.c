string16 ExtensionInstallUI::Prompt::GetUserCount() const {
  CHECK_EQ(INLINE_INSTALL_PROMPT, type_);
  return l10n_util::GetStringFUTF16(
      IDS_EXTENSION_USER_COUNT,
      UTF8ToUTF16(localized_user_count_));
}
