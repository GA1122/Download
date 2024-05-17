bool TranslateManager::IsSupportedLanguage(const std::string& page_language) {
  if (supported_languages_.Pointer()->empty()) {
    for (size_t i = 0; i < arraysize(kSupportedLanguages); ++i)
      supported_languages_.Pointer()->insert(kSupportedLanguages[i]);
  }
  return supported_languages_.Pointer()->find(page_language) !=
      supported_languages_.Pointer()->end();
}
