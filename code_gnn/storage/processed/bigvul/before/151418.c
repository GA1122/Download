ClientHintsPreferences FrameFetchContext::GetClientHintsPreferences() const {
  if (IsDetached())
    return frozen_state_->client_hints_preferences;

  if (!document_)
    return ClientHintsPreferences();

  return document_->GetClientHintsPreferences();
}
