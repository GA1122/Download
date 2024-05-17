void PasswordAccessoryControllerImpl::RefreshSuggestionsForField(
    const url::Origin& origin,
    bool is_fillable,
    bool is_password_field) {
  current_origin_ = is_fillable ? origin : url::Origin();
  GetManualFillingController()->RefreshSuggestionsForField(
      is_fillable, CreateAccessorySheetData(
                       origin,
                       is_fillable ? origin_suggestions_[origin]
                                   : std::vector<SuggestionElementData>(),
                       is_password_field));
  if (is_password_field) {
    GetManualFillingController()->ShowWhenKeyboardIsVisible(
        FillingSource::PASSWORD_FALLBACKS);
  } else {
    GetManualFillingController()->Hide(FillingSource::PASSWORD_FALLBACKS);
  }
}
