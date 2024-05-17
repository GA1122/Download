bool CanShowSuggestion(const PasswordFormFillData& fill_data,
                       const base::string16& current_username,
                       bool show_all) {
  base::string16 current_username_lower = base::i18n::ToLower(current_username);
  for (const auto& usernames : fill_data.other_possible_usernames) {
    for (size_t i = 0; i < usernames.second.size(); ++i) {
      if (show_all ||
          base::StartsWith(
              base::i18n::ToLower(base::string16(usernames.second[i])),
              current_username_lower, base::CompareCase::SENSITIVE)) {
        return true;
      }
    }
  }

  if (show_all ||
      base::StartsWith(base::i18n::ToLower(fill_data.username_field.value),
                       current_username_lower, base::CompareCase::SENSITIVE)) {
    return true;
  }

  for (const auto& login : fill_data.additional_logins) {
    if (show_all ||
        base::StartsWith(base::i18n::ToLower(login.first),
                         current_username_lower,
                         base::CompareCase::SENSITIVE)) {
      return true;
    }
  }

  return false;
}
