bool PasswordAutofillManager::FillSuggestion(int key,
                                             const base::string16& username) {
  autofill::PasswordFormFillData fill_data;
  autofill::PasswordAndRealm password_and_realm;
  if (FindLoginInfo(key, &fill_data) &&
      GetPasswordAndRealmForUsername(
          username, fill_data, &password_and_realm)) {
    bool is_android_credential = FacetURI::FromPotentiallyInvalidSpec(
        password_and_realm.realm).IsValidAndroidFacetURI();
    metrics_util::LogFilledCredentialIsFromAndroidApp(is_android_credential);
    password_manager_driver_->FillSuggestion(
        username, password_and_realm.password);
    return true;
  }
  return false;
}
