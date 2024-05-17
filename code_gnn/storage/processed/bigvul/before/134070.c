void ExtensionRegistry::TriggerOnLoaded(const Extension* extension) {
  CHECK(extension);
  DCHECK(enabled_extensions_.Contains(extension->id()));
  FOR_EACH_OBSERVER(ExtensionRegistryObserver,
                    observers_,
                    OnExtensionLoaded(browser_context_, extension));
}
