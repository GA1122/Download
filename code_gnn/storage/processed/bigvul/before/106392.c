bool PluginSelectionPolicy::Init() {
  return InitFromFile(FilePath(kPluginSelectionPolicyFile));
}
