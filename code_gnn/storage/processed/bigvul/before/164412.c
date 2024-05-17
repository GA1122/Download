const PermissionSet* PermissionsData::GetTabSpecificPermissions(
    int tab_id) const {
  runtime_lock_.AssertAcquired();
  TabPermissionsMap::const_iterator iter =
      tab_specific_permissions_.find(tab_id);
  return iter != tab_specific_permissions_.end() ? iter->second.get() : nullptr;
}
