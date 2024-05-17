ContentSettingsClient* LocalFrame::GetContentSettingsClient() {
  return Client() ? &Client()->GetContentSettingsClient() : nullptr;
}
