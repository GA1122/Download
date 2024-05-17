HRESULT CGaiaCredentialBase::SetSelected(BOOL* auto_login) {
  *auto_login = CanAttemptWindowsLogon();
  LOGFN(INFO) << "auto-login=" << *auto_login;

  DeleteStartupSentinel();
  return S_OK;
}
