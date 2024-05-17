HRESULT CGaiaCredentialBase::HandleAutologon(
    CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE* cpgsr,
    CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* cpcs) {
  USES_CONVERSION;
  LOGFN(INFO) << "user-sid=" << get_sid().m_str;
  DCHECK(cpgsr);
  DCHECK(cpcs);

  if (!CanAttemptWindowsLogon())
    return S_FALSE;

  if (needs_windows_password_) {
    HRESULT hr = IsWindowsPasswordValidForStoredUser(current_windows_password_);
    if (hr == S_OK) {
      OSUserManager* manager = OSUserManager::Get();
      hr = manager->ChangeUserPassword(domain_, username_,
                                       current_windows_password_, password_);
      if (FAILED(hr)) {
        if (hr != HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)) {
          LOGFN(ERROR) << "ChangeUserPassword hr=" << putHR(hr);
          return hr;
        }
        LOGFN(ERROR) << "Access was denied to ChangeUserPassword.";
        password_ = current_windows_password_;
      }
    } else {
      if (current_windows_password_.Length() && events_) {
        UINT pasword_message_id = IDS_INVALID_PASSWORD_BASE;
        if (hr == HRESULT_FROM_WIN32(ERROR_ACCOUNT_LOCKED_OUT)) {
          pasword_message_id = IDS_ACCOUNT_LOCKED_BASE;
          LOGFN(ERROR) << "Account is locked.";
        }

        events_->SetFieldString(this, FID_DESCRIPTION,
                                GetStringResource(pasword_message_id).c_str());
        events_->SetFieldInteractiveState(this, FID_CURRENT_PASSWORD_FIELD,
                                          CPFIS_FOCUSED);
      }
      return S_FALSE;
    }
  }

  HRESULT hr =
      AssociatedUserValidator::Get()->RestoreUserAccess(OLE2W(get_sid()));
  if (FAILED(hr) && hr != HRESULT_FROM_NT(STATUS_OBJECT_NAME_NOT_FOUND)) {
    LOGFN(ERROR) << "RestoreUserAccess hr=" << putHR(hr);
    return hr;
  }

  DWORD cpus = 0;
  provider()->GetUsageScenario(&cpus);
  hr = BuildCredPackAuthenticationBuffer(
      domain_, get_username(), get_password(),
      static_cast<CREDENTIAL_PROVIDER_USAGE_SCENARIO>(cpus), cpcs);
  if (FAILED(hr)) {
    LOGFN(ERROR) << "BuildCredPackAuthenticationBuffer hr=" << putHR(hr);
    return hr;
  }

  cpcs->clsidCredentialProvider = CLSID_GaiaCredentialProvider;
  *cpgsr = CPGSR_RETURN_CREDENTIAL_FINISHED;

  return S_OK;
}
