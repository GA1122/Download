bool ContainsWidevine(
    const std::vector<std::unique_ptr<media::KeySystemProperties>>&
        key_systems_properties) {
  for (const auto& key_system_properties : key_systems_properties) {
    if (key_system_properties->GetKeySystemName() == kWidevineKeySystem)
      return true;
  }
  return false;
}
