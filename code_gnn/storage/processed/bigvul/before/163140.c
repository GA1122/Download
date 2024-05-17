PluginSet& PluginsPendingDispose() {
  DEFINE_STATIC_LOCAL(PluginSet, set, ());
  return set;
}
