std::vector<ContentSettingsType> PageInfo::GetAllPermissionsForTesting() {
  std::vector<ContentSettingsType> permission_list;
  for (size_t i = 0; i < base::size(kPermissionType); ++i) {
#if !defined(OS_ANDROID)
    if (kPermissionType[i] == CONTENT_SETTINGS_TYPE_AUTOPLAY)
      continue;
#endif
    permission_list.push_back(kPermissionType[i]);
  }
   return permission_list;
 }
