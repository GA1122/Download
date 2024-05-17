bool FrameFetchContext::AllowScriptFromSource(const KURL& url) const {
  ContentSettingsClient* settings_client = GetContentSettingsClient();
  Settings* settings = GetSettings();
  if (settings_client && !settings_client->AllowScriptFromSource(
                             !settings || settings->GetScriptEnabled(), url)) {
    settings_client->DidNotAllowScript();
    return false;
  }
  return true;
}
