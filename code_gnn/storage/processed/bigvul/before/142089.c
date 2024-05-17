void CGaiaCredentialBase::UpdateSubmitButtonInteractiveState() {
  if (events_) {
    bool should_enable =
        logon_ui_process_ == INVALID_HANDLE_VALUE &&
        (!needs_windows_password_ || current_windows_password_.Length());
    events_->SetFieldInteractiveState(
        this, FID_SUBMIT, should_enable ? CPFIS_NONE : CPFIS_DISABLED);
  }
}
