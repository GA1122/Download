bool PermissionsData::HasTabSpecificPermissionToExecuteScript(
    int tab_id,
    const GURL& url) const {
  if (tab_id >= 0) {
    scoped_refptr<const PermissionSet> tab_permissions =
        GetTabSpecificPermissions(tab_id);
    if (tab_permissions.get() &&
        tab_permissions->explicit_hosts().MatchesSecurityOrigin(url)) {
      return true;
    }
  }
  return false;
}
