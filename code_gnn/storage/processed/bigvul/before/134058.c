scoped_ptr<ExtensionSet> ExtensionRegistry::GenerateInstalledExtensionsSet(
    int include_mask) const {
  scoped_ptr<ExtensionSet> installed_extensions(new ExtensionSet);
  if (include_mask & IncludeFlag::ENABLED)
    installed_extensions->InsertAll(enabled_extensions_);
  if (include_mask & IncludeFlag::DISABLED)
    installed_extensions->InsertAll(disabled_extensions_);
  if (include_mask & IncludeFlag::TERMINATED)
    installed_extensions->InsertAll(terminated_extensions_);
  if (include_mask & IncludeFlag::BLACKLISTED)
    installed_extensions->InsertAll(blacklisted_extensions_);
  if (include_mask & IncludeFlag::BLOCKED)
    installed_extensions->InsertAll(blocked_extensions_);
  return installed_extensions.Pass();
}
