void GetSuggestions(const autofill::PasswordFormFillData& fill_data,
                    const base::string16& current_username,
                    std::vector<autofill::Suggestion>* suggestions,
                    bool show_all,
                    bool is_password_field) {
  AppendSuggestionIfMatching(fill_data.username_field.value, current_username,
                             fill_data.preferred_realm, show_all,
                             is_password_field, suggestions);

  for (const auto& login : fill_data.additional_logins) {
    AppendSuggestionIfMatching(login.first, current_username,
                               login.second.realm, show_all, is_password_field,
                               suggestions);
  }

  for (const auto& usernames : fill_data.other_possible_usernames) {
    for (size_t i = 0; i < usernames.second.size(); ++i) {
      AppendSuggestionIfMatching(usernames.second[i], current_username,
                                 usernames.first.realm, show_all,
                                 is_password_field, suggestions);
    }
  }

  if (autofill::IsFeatureSubstringMatchEnabled()) {
    std::sort(suggestions->begin(), suggestions->end(),
              [](const autofill::Suggestion& a, const autofill::Suggestion& b) {
                return a.match < b.match;
              });
  }
}
