void ExtensionRegistry::ClearAll() {
  enabled_extensions_.Clear();
  disabled_extensions_.Clear();
  terminated_extensions_.Clear();
  blacklisted_extensions_.Clear();
  blocked_extensions_.Clear();
}
