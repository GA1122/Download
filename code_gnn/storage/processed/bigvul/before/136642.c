bool FrameLoader::AllowPlugins(ReasonForCallingAllowPlugins reason) {
  if (!Client())
    return false;
  Settings* settings = frame_->GetSettings();
  bool allowed = settings && settings->GetPluginsEnabled();
  if (!allowed && reason == kAboutToInstantiatePlugin)
    frame_->GetContentSettingsClient()->DidNotAllowPlugins();
  return allowed;
}
