void CGaiaCredentialBase::DisplayErrorInUI(LONG status,
                                           LONG substatus,
                                           BSTR status_text) {
  if (status != STATUS_SUCCESS) {
    if (events_)
      events_->SetFieldString(this, FID_DESCRIPTION, status_text);
  }
}
