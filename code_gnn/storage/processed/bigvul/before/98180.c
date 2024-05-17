void TranslateManager::GetSupportedLanguages(
    std::vector<std::string>* languages) {
  DCHECK(languages && languages->empty());
  for (size_t i = 0; i < arraysize(kSupportedLanguages); ++i)
    languages->push_back(kSupportedLanguages[i]);
}
