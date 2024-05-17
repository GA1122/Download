ProfileKeyedService* TestExtensionSystem::Build(Profile* profile) {
  return new TestExtensionSystem(profile);
}
