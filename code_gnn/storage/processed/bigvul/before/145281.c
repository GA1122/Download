void InitDetectedLanguages(
    CLD2::Language* languages,
    int* percents,
    std::vector<scoped_ptr<DetectedLanguage>>* detected_languages) {
  for (int i = 0; i < kCldNumLangs; i++) {
    std::string language_code;
    if (languages[i] == CLD2::UNKNOWN_LANGUAGE) {
      break;
    } else {
      language_code =
          CLD2::LanguageCode(static_cast<CLD2::Language>(languages[i]));
    }
    detected_languages->push_back(
        make_scoped_ptr(new DetectedLanguage(language_code, percents[i])));
  }
}
