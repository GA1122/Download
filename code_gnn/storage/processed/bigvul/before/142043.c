bool CGaiaCredentialBase::AreCredentialsValid() const {
  return CanAttemptWindowsLogon() &&
         IsWindowsPasswordValidForStoredUser(password_) == S_OK;
}
