  bool IsPrivilegeIncrease(const APIPermissionSet& old_permissions,
                           const APIPermissionSet& new_permissions) {
    return message_provider_->IsPrivilegeIncrease(
        PermissionSet(old_permissions, ManifestPermissionSet(), URLPatternSet(),
                      URLPatternSet()),
        PermissionSet(new_permissions, ManifestPermissionSet(), URLPatternSet(),
                      URLPatternSet()),
         Manifest::TYPE_EXTENSION);
   }
