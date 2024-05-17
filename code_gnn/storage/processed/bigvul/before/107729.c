bool FrameLoader::AllowPlugins(ReasonForCallingAllowPlugins reason) {
  if (!Client())
    return false;
  Settings* settings = frame_->GetSettings();
  bool allowed = settings && settings->GetPluginsEnabled();
  if (!allowed && reason == kAboutToInstantiatePlugin) {
    if (auto* settings_client = frame_->GetContentSettingsClient())
      settings_client->DidNotAllowPlugins();
  }
  return allowed;
}
