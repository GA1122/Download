static void CreateValidFileNameFromTitle(const GURL& url,
                                         const base::string16& title,
                                         base::string16* validated) {
  if (title.empty()) {
    if (url.is_valid()) {
      *validated = net::GetSuggestedFilename(url, "", "", "", "",
                                             std::string());
    } else {
      *validated =
          l10n_util::GetStringUTF16(IDS_APP_UNTITLED_SHORTCUT_FILE_NAME);
    }
  } else {
    *validated = title;
    file_util::ReplaceIllegalCharactersInPath(validated, '-');
  }
  static const wchar_t extension[] = L".url";
  static const size_t max_length = MAX_PATH - arraysize(extension);
  if (validated->size() > max_length)
    validated->erase(max_length);
  *validated += extension;
}
