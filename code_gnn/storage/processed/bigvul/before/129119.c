bool PermissionsData::HasHostPermission(const GURL& url) const {
  return active_permissions()->HasExplicitAccessToOrigin(url);
}
