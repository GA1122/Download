HRESULT CGaiaCredentialBase::Initialize(IGaiaCredentialProvider* provider) {
  LOGFN(INFO);
  DCHECK(provider);

  provider_ = provider;
  return S_OK;
}
