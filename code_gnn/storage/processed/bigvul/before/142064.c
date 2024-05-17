HRESULT CGaiaCredentialBase::GetSerialization(
    CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE* cpgsr,
    CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* cpcs,
    wchar_t** status_text,
    CREDENTIAL_PROVIDER_STATUS_ICON* status_icon) {
  USES_CONVERSION;
  LOGFN(INFO);
  DCHECK(status_text);
  DCHECK(status_icon);

  *status_text = nullptr;
  *status_icon = CPSI_NONE;
  memset(cpcs, 0, sizeof(*cpcs));

  if (events_) {
    events_->SetFieldInteractiveState(this, FID_SUBMIT, CPFIS_DISABLED);
    events_->SetFieldInteractiveState(this, FID_CURRENT_PASSWORD_FIELD,
                                      CPFIS_DISABLED);
  }

  HRESULT hr = HandleAutologon(cpgsr, cpcs);

  if (FAILED(hr)) {
    LOGFN(ERROR) << "HandleAutologon hr=" << putHR(hr);
    *status_icon = CPSI_ERROR;
    *cpgsr = CPGSR_RETURN_NO_CREDENTIAL_FINISHED;
  } else if (hr == S_FALSE) {

    if (logon_ui_process_ != INVALID_HANDLE_VALUE || needs_windows_password_) {
      *cpgsr = CPGSR_NO_CREDENTIAL_NOT_FINISHED;

      if (needs_windows_password_)
        *status_icon = CPSI_WARNING;

      hr = S_OK;
    } else {
      LOGFN(INFO) << "HandleAutologon hr=" << putHR(hr);
      TellOmahaDidRun();

      if (!InternetAvailabilityChecker::Get()->HasInternetConnection()) {
        BSTR error_message = AllocErrorString(IDS_NO_NETWORK_BASE);
        ::SHStrDupW(OLE2CW(error_message), status_text);
        ::SysFreeString(error_message);

        *status_icon = CPSI_NONE;
        *cpgsr = CPGSR_NO_CREDENTIAL_FINISHED;
        LOGFN(INFO) << "No internet connection";
        UpdateSubmitButtonInteractiveState();

        hr = S_OK;
      } else {
        *cpgsr = CPGSR_NO_CREDENTIAL_NOT_FINISHED;

        hr = CreateAndRunLogonStub();
      }
    }
  } else {
    *status_icon = CPSI_SUCCESS;
  }

  if (*cpgsr != CPGSR_NO_CREDENTIAL_FINISHED &&
      *cpgsr != CPGSR_RETURN_CREDENTIAL_FINISHED &&
      *cpgsr != CPGSR_RETURN_NO_CREDENTIAL_FINISHED) {
    if (events_) {
      events_->SetFieldInteractiveState(
          this, FID_CURRENT_PASSWORD_FIELD,
          needs_windows_password_ ? CPFIS_FOCUSED : CPFIS_NONE);
    }
    UpdateSubmitButtonInteractiveState();
  }
  return hr;
}
