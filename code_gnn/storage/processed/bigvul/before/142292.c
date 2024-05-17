ChromePasswordManagerClient::GetPasswordStore() const {
  return PasswordStoreFactory::GetForProfile(
             profile_, ServiceAccessType::EXPLICIT_ACCESS).get();
}
