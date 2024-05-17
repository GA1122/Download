void ExtensionRegistry::TriggerOnWillBeInstalled(const Extension* extension,
                                                 bool is_update,
                                                 bool from_ephemeral,
                                                 const std::string& old_name) {
  CHECK(extension);
  DCHECK_EQ(is_update,
            GenerateInstalledExtensionsSet()->Contains(extension->id()));
  DCHECK_EQ(is_update, !old_name.empty());
  FOR_EACH_OBSERVER(
      ExtensionRegistryObserver,
      observers_,
      OnExtensionWillBeInstalled(
          browser_context_, extension, is_update, from_ephemeral, old_name));
}
