HRESULT CGaiaCredentialBase::ReportError(LONG status,
                                         LONG substatus,
                                         BSTR status_text) {
  USES_CONVERSION;
  LOGFN(INFO);

  result_status_ = status;

  TerminateLogonProcess();
  UpdateSubmitButtonInteractiveState();

  DisplayErrorInUI(status, STATUS_SUCCESS, status_text);

  return provider_->OnUserAuthenticated(nullptr, CComBSTR(), CComBSTR(),
                                        CComBSTR(), FALSE);
}
