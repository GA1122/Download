bool CGaiaCredentialBase::CanAttemptWindowsLogon() const {
  return username_.Length() > 0 && password_.Length() > 0;
}
