bool PermissionsData::CanCaptureVisiblePage(int tab_id,
                                            std::string* error) const {
  const URLPattern all_urls(URLPattern::SCHEME_ALL,
                            URLPattern::kAllUrlsPattern);

  if (active_permissions()->explicit_hosts().ContainsPattern(all_urls))
    return true;

  if (tab_id >= 0) {
    scoped_refptr<const PermissionSet> tab_permissions =
        GetTabSpecificPermissions(tab_id);
    if (tab_permissions &&
        tab_permissions->HasAPIPermission(APIPermission::kTab)) {
      return true;
    }
    if (error)
      *error = manifest_errors::kActiveTabPermissionNotGranted;
    return false;
  }

  if (error)
    *error = manifest_errors::kAllURLOrActiveTabNeeded;
  return false;
}
