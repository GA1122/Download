const ClientHintsPreferences FrameFetchContext::GetClientHintsPreferences()
    const {
  if (IsDetached())
    return frozen_state_->client_hints_preferences;

  if (!document_ || !document_->GetFrame())
    return ClientHintsPreferences();

  return document_->GetFrame()->GetClientHintsPreferences();
}
