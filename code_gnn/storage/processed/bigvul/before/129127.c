void PermissionsData::UpdateTabSpecificPermissions(
    int tab_id,
    scoped_refptr<const PermissionSet> permissions) const {
  base::AutoLock auto_lock(runtime_lock_);
  CHECK_GE(tab_id, 0);
  TabPermissionsMap::iterator iter = tab_specific_permissions_.find(tab_id);
  if (iter == tab_specific_permissions_.end())
    tab_specific_permissions_[tab_id] = permissions;
  else
    iter->second = PermissionSet::CreateUnion(iter->second, permissions);
}
