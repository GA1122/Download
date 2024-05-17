base::string16 ExtensionInstallPrompt::Prompt::GetRatingCount() const {
  CHECK(AllowWebstoreData(type_));
  return l10n_util::GetStringFUTF16(IDS_EXTENSION_RATING_COUNT,
                                    base::IntToString16(rating_count_));
}
