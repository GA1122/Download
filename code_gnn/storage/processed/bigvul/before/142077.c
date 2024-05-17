void CGaiaCredentialBase::ResetInternalState() {
  LOGFN(INFO);
  username_.Empty();
  domain_.Empty();
  password_.Empty();
  current_windows_password_.Empty();
  authentication_results_.reset();
  needs_windows_password_ = false;
  result_status_ = STATUS_SUCCESS;

  TerminateLogonProcess();

  if (events_) {
    wchar_t* default_status_text = nullptr;
    GetStringValue(FID_DESCRIPTION, &default_status_text);
    events_->SetFieldString(this, FID_DESCRIPTION, default_status_text);
    events_->SetFieldState(this, FID_CURRENT_PASSWORD_FIELD, CPFS_HIDDEN);
    events_->SetFieldString(this, FID_CURRENT_PASSWORD_FIELD,
                            current_windows_password_);
    events_->SetFieldSubmitButton(this, FID_SUBMIT, FID_DESCRIPTION);
    UpdateSubmitButtonInteractiveState();
  }
}
