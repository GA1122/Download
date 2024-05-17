string16 ExtensionInstallUI::Prompt::GetRatingCount() const {
  CHECK_EQ(INLINE_INSTALL_PROMPT, type_);
  return l10n_util::GetStringFUTF16(
        IDS_EXTENSION_RATING_COUNT,
        UTF8ToUTF16(base::IntToString(rating_count_)));
}
