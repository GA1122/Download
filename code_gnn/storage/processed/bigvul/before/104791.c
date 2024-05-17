void ExtensionService::DisableIfPrivilegeIncrease(const Extension* extension) {
  const Extension* old = GetExtensionByIdInternal(extension->id(),
                                                  true, true, false);
  bool granted_full_access;
  std::set<std::string> granted_apis;
  URLPatternSet granted_extent;

  bool is_extension_upgrade = old != NULL;
  bool is_privilege_increase = false;

  if (extension->location() == Extension::INTERNAL) {
    if (!extension_prefs_->GetGrantedPermissions(extension->id(),
                                                 &granted_full_access,
                                                 &granted_apis,
                                                 &granted_extent)) {
      GrantPermissions(extension);
      CHECK(extension_prefs_->GetGrantedPermissions(extension->id(),
                                                    &granted_full_access,
                                                    &granted_apis,
                                                    &granted_extent));
    }

    is_privilege_increase = Extension::IsPrivilegeIncrease(
        granted_full_access, granted_apis, granted_extent, extension);
  }

  if (is_extension_upgrade) {
    if (extension->location() != Extension::LOAD)
      CHECK(extension->version()->CompareTo(*(old->version())) >= 0);

    if (!is_privilege_increase) {
      SetBeingUpgraded(old, true);
      SetBeingUpgraded(extension, true);
    }

    UnloadExtension(old->id(), UnloadedExtensionInfo::UPDATE);
    old = NULL;
  }

  if (is_privilege_increase) {
    if (!extension_prefs_->DidExtensionEscalatePermissions(extension->id())) {
      RecordPermissionMessagesHistogram(
          extension, "Extensions.Permissions_AutoDisable");
    }
    extension_prefs_->SetExtensionState(extension->id(), Extension::DISABLED);
    extension_prefs_->SetDidExtensionEscalatePermissions(extension, true);
  }
}
