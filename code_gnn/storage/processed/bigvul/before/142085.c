HRESULT CGaiaCredentialBase::Terminate() {
  LOGFN(INFO);
  SetDeselected();
  provider_.Release();
  return S_OK;
}
