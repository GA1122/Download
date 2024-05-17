PluginModuleSet* GetLivePluginSet() {
  CR_DEFINE_STATIC_LOCAL(PluginModuleSet, live_plugin_libs, ());
  return &live_plugin_libs;
}
