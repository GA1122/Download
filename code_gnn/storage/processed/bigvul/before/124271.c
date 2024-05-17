ExtensionSystem* ExtensionSystem::Get(Profile* profile) {
  return ExtensionSystemFactory::GetForProfile(profile);
}
