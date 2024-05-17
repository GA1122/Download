void ExtensionRegistry::TriggerOnUnloaded(
    const Extension* extension,
    UnloadedExtensionInfo::Reason reason) {
  CHECK(extension);
  DCHECK(!enabled_extensions_.Contains(extension->id()));
  FOR_EACH_OBSERVER(ExtensionRegistryObserver,
                    observers_,
                    OnExtensionUnloaded(browser_context_, extension, reason));
}
