void ExtensionRegistry::SetDisabledModificationCallback(
    const ExtensionSet::ModificationCallback& callback) {
  disabled_extensions_.set_modification_callback(callback);
}
