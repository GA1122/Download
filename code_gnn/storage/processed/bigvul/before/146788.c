void Document::LoadPluginsSoon() {
  if (!plugin_loading_timer_.IsActive())
    plugin_loading_timer_.StartOneShot(0, BLINK_FROM_HERE);
}
