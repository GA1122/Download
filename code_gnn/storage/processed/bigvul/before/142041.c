HRESULT CGaiaCredentialBase::Advise(ICredentialProviderCredentialEvents* cpce) {
  LOGFN(INFO);
  events_ = cpce;
  return S_OK;
}
