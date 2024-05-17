UserAgentSettingsIOData* BrowserContextIOData::GetUserAgentSettings() const {
  return GetSharedData().user_agent_settings.get();
}
