FrameFetchContext::GetFetchClientSettingsObject() const {
  DCHECK(fetch_client_settings_object_);
  return fetch_client_settings_object_.Get();
}
