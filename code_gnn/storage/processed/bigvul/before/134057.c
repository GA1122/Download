scoped_ptr<ExtensionSet> ExtensionRegistry::GenerateInstalledExtensionsSet()
    const {
  return GenerateInstalledExtensionsSet(EVERYTHING).Pass();
}
