bool HasExactMatchSuggestion(const PasswordFormFillData& fill_data,
                             const base::string16& current_username) {
  if (fill_data.username_field.value == current_username)
    return true;

  for (const auto& usernames : fill_data.other_possible_usernames) {
    for (const auto& username_string : usernames.second) {
      if (username_string == current_username)
        return true;
    }
  }

  for (const auto& login : fill_data.additional_logins) {
    if (login.first == current_username)
      return true;
  }

  return false;
}
