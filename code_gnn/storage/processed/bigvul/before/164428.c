void PermissionsData::UpdateTabSpecificPermissions(
    int tab_id,
    const PermissionSet& permissions) const {
  AutoLockOnValidThread lock(runtime_lock_, thread_checker_.get());
  CHECK_GE(tab_id, 0);
  TabPermissionsMap::const_iterator iter =
      tab_specific_permissions_.find(tab_id);
  std::unique_ptr<const PermissionSet> new_permissions =
      PermissionSet::CreateUnion(
          iter == tab_specific_permissions_.end()
              ? static_cast<const PermissionSet&>(PermissionSet())
              : *iter->second,
          permissions);
  tab_specific_permissions_[tab_id] = std::move(new_permissions);
}
