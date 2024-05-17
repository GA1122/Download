ExtensionSystem::ExtensionSystem() {
  if (Feature::GetCurrentChannel() == Feature::GetDefaultChannel())
    Feature::SetCurrentChannel(chrome::VersionInfo::GetChannel());
}
