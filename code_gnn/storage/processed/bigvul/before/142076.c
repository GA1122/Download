HRESULT CGaiaCredentialBase::ReportResult(
    NTSTATUS status,
    NTSTATUS substatus,
    wchar_t** ppszOptionalStatusText,
    CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon) {
  LOGFN(INFO) << "status=" << putHR(status)
              << " substatus=" << putHR(substatus);

  if (status == STATUS_SUCCESS && authentication_results_) {
    authentication_results_->SetKey(
        kKeySID, base::Value(base::UTF16ToUTF8((BSTR)user_sid_)));
    authentication_results_->SetKey(
        kKeyDomain, base::Value(base::UTF16ToUTF8((BSTR)domain_)));
    authentication_results_->SetKey(
        kKeyUsername, base::Value(base::UTF16ToUTF8((BSTR)username_)));
    authentication_results_->SetKey(
        kKeyPassword, base::Value(base::UTF16ToUTF8((BSTR)password_)));

    CComBSTR status_text;
    HRESULT hr = ForkSaveAccountInfoStub(authentication_results_, &status_text);
    if (FAILED(hr))
      LOGFN(ERROR) << "ForkSaveAccountInfoStub hr=" << putHR(hr);
  }

  *ppszOptionalStatusText = nullptr;
  *pcpsiOptionalStatusIcon = CPSI_NONE;
  ResetInternalState();
  return S_OK;
}
