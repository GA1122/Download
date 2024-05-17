void AppendSuggestionIfMatching(
    const base::string16& field_suggestion,
    const base::string16& field_contents,
    const std::string& signon_realm,
    bool show_all,
    bool is_password_field,
    std::vector<autofill::Suggestion>* suggestions) {
  base::string16 lower_suggestion = base::i18n::ToLower(field_suggestion);
  base::string16 lower_contents = base::i18n::ToLower(field_contents);
  bool prefix_matched_suggestion =
      show_all || base::StartsWith(lower_suggestion, lower_contents,
                                   base::CompareCase::SENSITIVE);
  if (prefix_matched_suggestion ||
      autofill::FieldIsSuggestionSubstringStartingOnTokenBoundary(
          lower_suggestion, lower_contents, true)) {
    autofill::Suggestion suggestion(ReplaceEmptyUsername(field_suggestion));
    suggestion.label = GetHumanReadableRealm(signon_realm);
    suggestion.frontend_id = is_password_field
                                 ? autofill::POPUP_ITEM_ID_PASSWORD_ENTRY
                                 : autofill::POPUP_ITEM_ID_USERNAME_ENTRY;
    suggestion.match = prefix_matched_suggestion
                           ? autofill::Suggestion::PREFIX_MATCH
                           : autofill::Suggestion::SUBSTRING_MATCH;
    suggestions->push_back(suggestion);
  }
}
