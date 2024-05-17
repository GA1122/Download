const SecurityOrigin* FrameFetchContext::GetSecurityOrigin() const {
  if (!fetch_client_settings_object_)
    return nullptr;
  return fetch_client_settings_object_->GetSecurityOrigin();
}
