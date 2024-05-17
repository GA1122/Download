std::string TranslateManager::GetLanguageCode(
    const std::string& chrome_locale) {
  for (size_t i = 0; i < arraysize(kLocaleToCLDLanguages); ++i) {
    if (chrome_locale == kLocaleToCLDLanguages[i].locale_language)
      return kLocaleToCLDLanguages[i].cld_language;
  }
  return chrome_locale;
}
