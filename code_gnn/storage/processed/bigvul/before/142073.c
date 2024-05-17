HRESULT CGaiaCredentialBase::OnDllUnregisterServer() {
  auto policy = ScopedLsaPolicy::Create(POLICY_ALL_ACCESS);
  if (policy) {
    wchar_t password[32];

    HRESULT hr = policy->RetrievePrivateData(kLsaKeyGaiaPassword, password,
                                             base::size(password));
    if (FAILED(hr))
      LOGFN(ERROR) << "policy.RetrievePrivateData hr=" << putHR(hr);

    hr = policy->RemovePrivateData(kLsaKeyGaiaPassword);
    if (FAILED(hr))
      LOGFN(ERROR) << "policy.RemovePrivateData hr=" << putHR(hr);

    OSUserManager* manager = OSUserManager::Get();
    PSID sid;

    wchar_t gaia_username[kWindowsUsernameBufferLength];
    hr = policy->RetrievePrivateData(kLsaKeyGaiaUsername, gaia_username,
                                     base::size(gaia_username));

    if (SUCCEEDED(hr)) {
      hr = policy->RemovePrivateData(kLsaKeyGaiaUsername);
      base::string16 local_domain = OSUserManager::GetLocalDomain();

      hr = manager->GetUserSID(local_domain.c_str(), gaia_username, &sid);
      if (FAILED(hr)) {
        LOGFN(ERROR) << "manager.GetUserSID hr=" << putHR(hr);
        sid = nullptr;
      }

      hr = manager->RemoveUser(gaia_username, password);
      if (FAILED(hr))
        LOGFN(ERROR) << "manager->RemoveUser hr=" << putHR(hr);

      if (sid != nullptr) {
        hr = policy->RemoveAccount(sid);
        ::LocalFree(sid);
        if (FAILED(hr))
          LOGFN(ERROR) << "policy.RemoveAccount hr=" << putHR(hr);
      }
    } else {
      LOGFN(ERROR) << "Get gaia username failed hr=" << putHR(hr);
    }

  } else {
    LOGFN(ERROR) << "ScopedLsaPolicy::Create failed";
  }

  return S_OK;
}
