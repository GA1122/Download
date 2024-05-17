void Document::LoadPluginsSoon() {
  if (!plugin_loading_timer_.IsActive())
    plugin_loading_timer_.StartOneShot(TimeDelta(), FROM_HERE);
}
