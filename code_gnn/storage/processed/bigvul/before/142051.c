void CGaiaCredentialBase::DisplayPasswordField(int password_message) {
  needs_windows_password_ = true;
  if (events_) {
    events_->SetFieldString(this, FID_DESCRIPTION,
                            GetStringResource(password_message).c_str());
    events_->SetFieldState(this, FID_CURRENT_PASSWORD_FIELD,
                           CPFS_DISPLAY_IN_SELECTED_TILE);
    events_->SetFieldInteractiveState(this, FID_CURRENT_PASSWORD_FIELD,
                                      CPFIS_FOCUSED);
    events_->SetFieldSubmitButton(this, FID_SUBMIT, FID_CURRENT_PASSWORD_FIELD);
  }
}
