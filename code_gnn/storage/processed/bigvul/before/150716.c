bool PermissionInfoListContainsPermission(const PermissionInfoList& permissions,
                                          ContentSettingsType content_type) {
  for (const auto& permission : permissions) {
    if (permission.type == content_type)
      return true;
  }
  return false;
}
