HRESULT CGaiaCredentialBase::UnAdvise(void) {
  LOGFN(INFO);
  events_.Release();

  return S_OK;
}
