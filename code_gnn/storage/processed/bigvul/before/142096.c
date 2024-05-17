HRESULT CreateCredentialWithProvider(
    IGaiaCredentialProvider* provider,
    IGaiaCredential** gaia_credential,
    ICredentialProviderCredential** credential) {
  HRESULT hr = CreateCredential(credential);
  if (SUCCEEDED(hr)) {
    hr = (*credential)
             ->QueryInterface(IID_IGaiaCredential,
                              reinterpret_cast<void**>(gaia_credential));
    if (SUCCEEDED(hr))
      hr = (*gaia_credential)->Initialize(provider);
  }
  return hr;
}
