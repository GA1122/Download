void ExtensionRegistry::TriggerOnInstalled(const Extension* extension,
                                           bool is_update) {
  CHECK(extension);
  DCHECK(GenerateInstalledExtensionsSet()->Contains(extension->id()));
  FOR_EACH_OBSERVER(ExtensionRegistryObserver,
                    observers_,
                    OnExtensionInstalled(
                        browser_context_, extension, is_update));
}
