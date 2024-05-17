void SupervisedUserService::ChangeExtensionStateIfNecessary(
    const std::string& extension_id) {
  ExtensionRegistry* registry = ExtensionRegistry::Get(profile_);
  const Extension* extension = registry->GetInstalledExtension(extension_id);
  if (!extension)
    return;

  ExtensionPrefs* extension_prefs = ExtensionPrefs::Get(profile_);
  extensions::ExtensionService* service =
      ExtensionSystem::Get(profile_)->extension_service();

  ExtensionState state = GetExtensionState(*extension);
  switch (state) {
    case ExtensionState::BLOCKED:
    case ExtensionState::FORCED:
      break;
    case ExtensionState::REQUIRE_APPROVAL:
      service->DisableExtension(
          extension_id,
          extensions::disable_reason::DISABLE_CUSTODIAN_APPROVAL_REQUIRED);
      break;
    case ExtensionState::ALLOWED:
      extension_prefs->RemoveDisableReason(
          extension_id,
          extensions::disable_reason::DISABLE_CUSTODIAN_APPROVAL_REQUIRED);
      extension_prefs->RemoveDisableReason(
          extension_id,
          extensions::disable_reason::DISABLE_PERMISSIONS_INCREASE);
      if (extension_prefs->GetDisableReasons(extension_id) ==
          extensions::disable_reason::DISABLE_NONE) {
        service->EnableExtension(extension_id);
      }
      break;
  }
}
