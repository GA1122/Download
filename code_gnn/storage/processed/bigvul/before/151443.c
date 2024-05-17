void FrameFetchContext::ParseAndPersistClientHints(
    const ResourceResponse& response) {
  ClientHintsPreferences hints_preferences;
  WebEnabledClientHints enabled_client_hints;
  TimeDelta persist_duration;
  FrameClientHintsPreferencesContext hints_context(GetFrame());
  hints_preferences.UpdatePersistentHintsFromHeaders(
      response, &hints_context, enabled_client_hints, &persist_duration);

  if (persist_duration.InSeconds() <= 0)
    return;

  GetContentSettingsClient()->PersistClientHints(
      enabled_client_hints, persist_duration, response.Url());
}
