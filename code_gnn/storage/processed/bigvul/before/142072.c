HRESULT CGaiaCredentialBase::OnDllRegisterServer() {
  OSUserManager* manager = OSUserManager::Get();

  auto policy = ScopedLsaPolicy::Create(POLICY_ALL_ACCESS);

  if (!policy) {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    LOGFN(ERROR) << "ScopedLsaPolicy::Create hr=" << putHR(hr);
    return hr;
  }

  PSID sid = nullptr;

  wchar_t gaia_username[kWindowsUsernameBufferLength];
  HRESULT hr = policy->RetrievePrivateData(kLsaKeyGaiaUsername, gaia_username,
                                           base::size(gaia_username));

  if (SUCCEEDED(hr)) {
    LOGFN(INFO) << "Expecting gaia user '" << gaia_username << "' to exist.";
    wchar_t password[32];
    HRESULT hr = policy->RetrievePrivateData(kLsaKeyGaiaPassword, password,
                                             base::size(password));
    if (SUCCEEDED(hr)) {
      base::string16 local_domain = OSUserManager::GetLocalDomain();
      base::win::ScopedHandle token;
      hr = OSUserManager::Get()->CreateLogonToken(
          local_domain.c_str(), gaia_username, password,
           false, &token);
      if (SUCCEEDED(hr)) {
        hr = OSUserManager::Get()->GetUserSID(local_domain.c_str(),
                                              gaia_username, &sid);
        if (FAILED(hr)) {
          LOGFN(ERROR) << "GetUserSID(sid from existing user '" << gaia_username
                       << "') hr=" << putHR(hr);
          sid = nullptr;
        }
      }
    }
  }

  if (sid == nullptr) {
    errno_t err = wcscpy_s(gaia_username, base::size(gaia_username),
                           kDefaultGaiaAccountName);
    if (err != 0) {
      LOGFN(ERROR) << "wcscpy_s errno=" << err;
      return E_FAIL;
    }

    wchar_t password[32];
    HRESULT hr =
        manager->GenerateRandomPassword(password, base::size(password));
    if (FAILED(hr)) {
      LOGFN(ERROR) << "GenerateRandomPassword hr=" << putHR(hr);
      return hr;
    }

    CComBSTR sid_string;
    CComBSTR gaia_username;
    hr =
        CreateNewUser(manager, kDefaultGaiaAccountName, password,
                      GetStringResource(IDS_GAIA_ACCOUNT_FULLNAME_BASE).c_str(),
                      GetStringResource(IDS_GAIA_ACCOUNT_COMMENT_BASE).c_str(),
                       false, kMaxUsernameAttempts,
                      &gaia_username, &sid_string);

    if (FAILED(hr)) {
      LOGFN(ERROR) << "CreateNewUser hr=" << putHR(hr);
      return hr;
    }

    if (!::ConvertStringSidToSid(sid_string, &sid)) {
      hr = HRESULT_FROM_WIN32(::GetLastError());
      LOGFN(ERROR) << "ConvertStringSidToSid hr=" << putHR(hr);
      return hr;
    }

    hr = policy->StorePrivateData(kLsaKeyGaiaPassword, password);
    if (FAILED(hr)) {
      LOGFN(ERROR) << "Failed to store gaia user password in LSA hr="
                   << putHR(hr);
      return hr;
    }

    hr = policy->StorePrivateData(kLsaKeyGaiaUsername, gaia_username);
    if (FAILED(hr)) {
      LOGFN(ERROR) << "Failed to store gaia user name in LSA hr=" << putHR(hr);
      return hr;
    }
  }

  if (!sid) {
    LOGFN(ERROR) << "No valid username could be found for the gaia user.";
    return HRESULT_FROM_WIN32(NERR_UserExists);
  }

  hr = policy->AddAccountRights(sid, SE_BATCH_LOGON_NAME);
  ::LocalFree(sid);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "policy.AddAccountRights hr=" << putHR(hr);
    return hr;
  }
  return S_OK;
}
