void ExtensionRegistry::TriggerOnUninstalled(const Extension* extension,
                                             UninstallReason reason) {
  CHECK(extension);
  DCHECK(!GenerateInstalledExtensionsSet()->Contains(extension->id()));
  FOR_EACH_OBSERVER(
      ExtensionRegistryObserver,
      observers_,
      OnExtensionUninstalled(browser_context_, extension, reason));
}
