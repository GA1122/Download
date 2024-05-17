SupervisedUserService::ExtensionState SupervisedUserService::GetExtensionState(
    const Extension& extension) const {
  bool was_installed_by_default = extension.was_installed_by_default();
#if defined(OS_CHROMEOS)
  was_installed_by_default =
      extensions::Manifest::IsExternalLocation(extension.location());
#endif
  if (extensions::Manifest::IsComponentLocation(extension.location()) ||
      extensions::Manifest::IsPolicyLocation(extension.location()) ||
      extension.is_theme() || extension.from_bookmark() ||
      extension.is_shared_module() || was_installed_by_default) {
    return ExtensionState::ALLOWED;
  }

  if (extensions::util::WasInstalledByCustodian(extension.id(), profile_))
    return ExtensionState::FORCED;

  if (!base::FeatureList::IsEnabled(
          supervised_users::kSupervisedUserInitiatedExtensionInstall)) {
    return ExtensionState::BLOCKED;
  }

  auto extension_it = approved_extensions_map_.find(extension.id());
  if (extension_it != approved_extensions_map_.end() &&
      extension_it->second == extension.version()) {
    return ExtensionState::ALLOWED;
  }
  return ExtensionState::REQUIRE_APPROVAL;
}
