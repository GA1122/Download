void AddSimpleSuggestionWithSeparatorOnTop(
    int value,
    int frontend_id,
    std::vector<autofill::Suggestion>* suggestions) {
#if !defined(OS_ANDROID)
  suggestions->push_back(autofill::Suggestion());
  suggestions->back().frontend_id = autofill::POPUP_ITEM_ID_SEPARATOR;
#endif

  autofill::Suggestion suggestion(l10n_util::GetStringUTF8(value),
                                  std::string(), std::string(), frontend_id);
  suggestions->push_back(suggestion);
}
