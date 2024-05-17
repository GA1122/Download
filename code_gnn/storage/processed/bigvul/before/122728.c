void Extension::RuntimeData::UpdateTabSpecificPermissions(
    int tab_id,
    scoped_refptr<const PermissionSet> permissions) {
  CHECK_GE(tab_id, 0);
  if (tab_specific_permissions_.count(tab_id)) {
    tab_specific_permissions_[tab_id] = PermissionSet::CreateUnion(
        tab_specific_permissions_[tab_id],
        permissions.get());
  } else {
    tab_specific_permissions_[tab_id] = permissions;
  }
}
