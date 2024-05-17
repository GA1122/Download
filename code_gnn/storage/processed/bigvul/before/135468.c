bool PasswordAutofillManager::GetPasswordAndRealmForUsername(
    const base::string16& current_username,
    const autofill::PasswordFormFillData& fill_data,
    autofill::PasswordAndRealm* password_and_realm) {

  if (fill_data.username_field.value == current_username) {
    password_and_realm->password = fill_data.password_field.value;
    password_and_realm->realm = fill_data.preferred_realm;
    return true;
  }

  for (autofill::PasswordFormFillData::LoginCollection::const_iterator iter =
           fill_data.additional_logins.begin();
       iter != fill_data.additional_logins.end(); ++iter) {
    if (iter->first == current_username) {
      *password_and_realm = iter->second;
      return true;
    }
  }

  for (autofill::PasswordFormFillData::UsernamesCollection::const_iterator
           usernames_iter = fill_data.other_possible_usernames.begin();
       usernames_iter != fill_data.other_possible_usernames.end();
       ++usernames_iter) {
    for (size_t i = 0; i < usernames_iter->second.size(); ++i) {
      if (usernames_iter->second[i] == current_username) {
        password_and_realm->password = usernames_iter->first.password;
        password_and_realm->realm = usernames_iter->first.realm;
        return true;
      }
    }
  }

  return false;
}
